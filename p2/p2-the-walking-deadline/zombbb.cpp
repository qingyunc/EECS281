// Project identifier 9504853406CBAC39EE89AA3AD238AA12CA198043

#include "P2random.h" ///
#include <algorithm>  ///
#include <deque>      ///
#include <getopt.h>   ///
#include <iostream>   ///
#include <queue>      ///
#include <string>     ///
#include <vector>     ///
using namespace std;

struct zombie { // check
  string name;
  uint32_t distance;
  uint32_t speed;
  uint32_t health;
  uint32_t num_active = 1;

  zombie(string name_input, uint32_t distance_input, uint32_t speed_input,
         uint32_t health_input)
      : name(name_input), distance(distance_input), speed(speed_input),
        health(health_input) {}
};

struct SortBy_eta { // check
  bool operator()(const zombie *const &lhs, const zombie *const &rhs) const {
    auto &&lhs_eta = lhs->distance / lhs->speed;
    auto &&rhs_eta = rhs->distance / rhs->speed;
    if (lhs_eta == rhs_eta) {
      if (lhs->health == rhs->health) {
        return lhs->name > rhs->name;

      } else {
        return lhs->health > rhs->health;
      }
    }
    return lhs_eta > rhs_eta;
  }
};

struct more_active_time { // check
  bool operator()(const zombie *lhs, const zombie *rhs) const {
    if (lhs->num_active == rhs->num_active) {
      return lhs->name > rhs->name;
    }
    return lhs->num_active < rhs->num_active;
  }
};

struct less_active_time { // check 可能出错
  bool operator()(const zombie *lhs, const zombie *rhs) const {
    if (lhs->num_active == rhs->num_active) {
      return lhs->name > rhs->name;
    }
    return lhs->num_active > rhs->num_active;
  }
};

struct median { // check
  priority_queue<uint32_t, vector<uint32_t>, std::greater<uint32_t>> big_heap;
  priority_queue<uint32_t, vector<uint32_t>, std::less<uint32_t>> small_heap;

  bool empty() { return big_heap.empty() && small_heap.empty(); }

  void add(uint32_t num) {
    if (small_heap.empty() && big_heap.empty()) {
      small_heap.push(num);
    } else {
      if (num > small_heap.top()) {
        big_heap.push(num);
      } else {
        small_heap.push(num);
      }
    }

    if (small_heap.size() - big_heap.size() == 2) {
      big_heap.push(small_heap.top());
      small_heap.pop();
    } else if (big_heap.size() - small_heap.size() == 2) {
      small_heap.push(big_heap.top());
      big_heap.pop();
    }
  }

  uint32_t get_median() {
    if (small_heap.size() == big_heap.size()) {
      return (small_heap.top() + big_heap.top()) / 2;
    } else if (small_heap.size() > big_heap.size()) {
      return small_heap.top();
    } else {
      return big_heap.top();
    }
  }
};

class zombbb {
public:
  string temp;
  deque<zombie> zombies;
  priority_queue<zombie *, vector<zombie *>, SortBy_eta> pq;

  priority_queue<zombie *, vector<zombie *>, more_active_time> longer;
  priority_queue<zombie *, vector<zombie *>, less_active_time> shorter;

  deque<zombie *> dead_zombies;

  uint32_t quiver_capacity;
  uint32_t num_quiver;

  uint32_t random_seed;
  uint32_t max_rand_distance;
  uint32_t max_rand_speed;
  uint32_t max_rand_health;

  bool player_active = true;
  uint32_t num_of_random_zombies;
  uint32_t num_of_name_zombies;
  uint32_t roun_temp;
  uint32_t roun_next;
  uint32_t roun_current = 1;
  string first_eat_my_brain;

  bool verbose = false;
  bool stats = false;
  bool median = false;
  uint32_t n = 0;

  void read_header() { //check
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cin >> temp >> quiver_capacity >> temp >> random_seed >> temp >>
        max_rand_distance >> temp >> max_rand_speed >> temp >>
        max_rand_health >> temp >> temp >> roun_next;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  void initialize() { //check
    P2random::initialize(random_seed, max_rand_distance, max_rand_speed,
                         max_rand_health);
  }

  void get_random_zombie() {
    while (num_of_random_zombies != 0) {
      string r_n = P2random::getNextZombieName();
      uint32_t r_d = P2random::getNextZombieDistance();
      uint32_t r_s = P2random::getNextZombieSpeed();
      uint32_t r_h = P2random::getNextZombieHealth();
      if (verbose) {
        cout << "Created: " << r_n << " (distance: " << r_d
             << ", speed: " << r_s << ", health: " << r_h << ")\n";
      }
      zombie zombie_random(r_n, r_d, r_s, r_h);
      zombies.push_back(zombie_random);
      pq.push(&zombies.back());
      num_of_random_zombies--;
    }
  }

  void get_name_zombie() {
    while (num_of_name_zombies != 0) {
      string n_n;
      string temp_1; 
      uint32_t n_d;
      uint32_t n_s;
      uint32_t n_h;
      cin >> n_n >> temp_1 >> n_d >> temp_1 >> n_s >> temp_1 >> n_h;
      if (verbose) {
        cout << "Created: " << n_n << " (distance: " << n_d
             << ", speed: " << n_s << ", health: " << n_h << ")\n";
      }
      zombie zombie_name(n_n, n_d, n_s, n_h);
      zombies.push_back(zombie_name);
      pq.push(&zombies.back());
      num_of_name_zombies--;
    }
  }

  uint32_t read_round() { // check
    cin >> temp >> num_of_random_zombies;
    get_random_zombie();
    cin >> temp >> num_of_name_zombies;
    get_name_zombie();
    cin >> temp >> temp >> roun_temp;
    return roun_temp;
  }

  void update() { // check
    deque<zombie>::iterator ita = zombies.begin();
    while (ita != zombies.end()) {
      if (pq.empty()) {
        break;
      }
      if (ita->distance <= ita->speed && ita->health != 0) {
        ita->distance = 0;
        if (player_active == true) {
          first_eat_my_brain = ita->name;
          player_active = false;
        }
        if (verbose) {
          cout << "Moved: " << ita->name << " (distance: " << ita->distance
               << ", speed: " << ita->speed << ", health: " << ita->health
               << ")\n";
        }
        ita->num_active++;
        ita++;
      } else if (ita->health != 0) {
        ita->distance -= ita->speed;
        if (verbose) {
          cout << "Moved: " << ita->name << " (distance: " << ita->distance
               << ", speed: " << ita->speed << ", health: " << ita->health
               << ")\n";
        }
        ita->num_active++;
        ita++;
      } else {
        ita++;
      }
    }
  }

  void attack(struct median &m) {
    while (num_quiver != 0) {
      if (pq.empty()) {
        break;
      }
      if (num_quiver >= pq.top()->health) {
        num_quiver -= pq.top()->health;
        pq.top()->health = 0;
        if (verbose) {
          cout << "Destroyed: " << pq.top()->name
               << " (distance: " << pq.top()->distance
               << ", speed: " << pq.top()->speed
               << ", health: " << pq.top()->health << ")" << endl;
        }
        if (stats) {
          dead_zombies.push_back(pq.top());
        }
        if (median) {
          m.add(pq.top()->num_active);
        }
        pq.pop();
      } else {
        pq.top()->health -= num_quiver;
        num_quiver = 0;
      }
    }
  }

  void whether_stat() {  // To do
    if (stats) {
      deque<zombie>::iterator i = zombies.begin();
      while (i != zombies.end()) {
        longer.push(&(*i));
        shorter.push(&(*i));
        i++;
      }
      cout << "Zombies still active: " << pq.size() << "\n";
    }
  }

  void stat_print() { // check

    uint32_t record_first = n;
    uint32_t record_temp = 1;

    uint32_t record_most = n;
    uint32_t record_least = n;
    deque<zombie *>::iterator k = dead_zombies.begin();
    deque<zombie *>::iterator k_back = dead_zombies.end() - 1;

    cout << "First zombies killed:"
         << "\n";

    while (record_first != 0 && k != dead_zombies.end()) { // 前n个杀死的
      cout << (*k)->name << " " << record_temp << "\n";
      record_temp++;
      k++;
      record_first--;
    }

    uint32_t record_last = min(n, static_cast<uint32_t>(dead_zombies.size()));
    cout << "Last zombies killed:"
         << "\n";
    while (record_last != 0) { // 后n个杀死的
      cout << (*k_back)->name << " " << record_last << "\n";
      record_last--;
      k_back--;
    }

    cout << "Most active zombies:"
         << "\n";
    while (record_most != 0 && (!longer.empty())) {
      cout << longer.top()->name << " " << longer.top()->num_active << "\n";
      longer.pop();
      record_most--;
    }

    cout << "Least active zombies:"
         << "\n";
    while (record_least != 0 && (!shorter.empty())) {
      cout << shorter.top()->name << " " << shorter.top()->num_active << "\n";
      shorter.pop();
      record_least--;
    }
  }

  void getMode(int argc, char *argv[]) { //check

    string mode;
    opterr = false;
    int choice;
    int option_index = 0;

    option long_options[] = {
        {"verbose", no_argument, nullptr, 'v'},
        {"statistics", required_argument, nullptr, 's'},
        {"median", no_argument, nullptr, 'm'},
        {"help", no_argument, nullptr, 'h'},
    };

    while ((choice = getopt_long(argc, argv, "vs:mh", long_options,
                                 &option_index)) != -1) {
      switch (choice) {
      case 'h':
        cout << "somthing"
             << "\n";
        exit(1);
        break;

      case 'v':
        verbose = true;
        break;

      case 's':
        stats =true;
        n = static_cast<uint32_t>(atoi(optarg));
        break;

      case 'm':
        median = true;
        break;

      default:
        cerr << "Unknown command line option" << '\n';
        exit(1);
      }
    }
  }
};

int main(int argc, char **argv) {
  median mid;
  auto z = zombbb();
  z.read_header();
  z.initialize();
  z.getMode(argc, argv);

  while (z.player_active) {
    if (z.verbose) {
      cout << "Round: " << z.roun_current << "\n";
    }
    z.num_quiver = z.quiver_capacity;
    z.update(); // updatd

    if (!z.player_active) {
      cout << "DEFEAT IN ROUND " << z.roun_current << "! "
           << z.first_eat_my_brain << " ate your brains!"
           << "\n";
      z.whether_stat();
      break;
    }

    if (z.roun_current == z.roun_next) { // generate
      z.roun_next = z.read_round();
    }

    z.num_quiver = z.quiver_capacity;

    z.attack(mid);

    if (!mid.empty() && z.median) {
      cout << "At the end of round " << z.roun_current
           << ", the median zombie lifetime is " << mid.get_median() << endl;
    }

    if (cin.fail() && z.pq.empty()) {
      cout << "VICTORY IN ROUND " << z.roun_current << "! "
           << z.dead_zombies.back()->name << " was the last zombie.\n";
      z.whether_stat();
      break;
    }
    z.roun_current++;
  }
  if (z.stats) {
    z.stat_print();
  }
  return 0;
}
