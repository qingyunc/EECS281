// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "TableEntry.h"
#include "getopt.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <deque>
#include <exception>
#include <getopt.h>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

struct header { // 地毯
  string column_name;
  EntryType column_type;
};

struct great_than { // 地毯
  TableEntry entry;
  int index;

  great_than(TableEntry entry, int index) : entry(entry), index(index) {}
  bool operator()(const vector<TableEntry> v) { return v[index] > entry; }
};

struct e { // 地毯
  TableEntry entry;
  int index;

  e(TableEntry entry, int index) : entry(entry), index(index) {}
  bool operator()(const vector<TableEntry> v) { return v[index] == entry; }
};

struct less_than { //地毯
  TableEntry entry;
  int index;

  less_than(TableEntry entry, int index) : entry(entry), index(index) {}
  bool operator()(const vector<TableEntry> v) { return v[index] < entry; }
};

class Table {
public:
  char i_type;  //
  int i_column; //

  vector<header> columns;          //
  vector<vector<TableEntry>> data; //

  map<TableEntry, vector<int>> bst_index;            //
  unordered_map<TableEntry, vector<int>> hash_index; //

  void table_init(int n) { // 地毯
    i_type = 0;
    i_column = -1;

    columns.resize(n);
    string s;

    for (int i = 0; i < n; i++) {
      cin >> s;
      if (s == "string") {
        columns[i].column_type = EntryType::String;
      } else if (s == "double") {
        columns[i].column_type = EntryType::Double;
      } else if (s == "int") {
        columns[i].column_type = EntryType::Int;
      } else if (s == "bool") {
        columns[i].column_type = EntryType::Bool;
      }
    }

    for (int i = 0; i < n; i++) {
      cin >> s;
      columns[i].column_name = s;
    }
  }

  TableEntry read(int idx) { // 地毯
    if (columns[idx].column_type == EntryType::Bool) {
      bool b;
      cin >> b;
      return TableEntry(b);
    } else if (columns[idx].column_type == EntryType::Int) {
      int i;
      cin >> i;
      return TableEntry(i);
    } else if (columns[idx].column_type == EntryType::Double) {
      double d;
      cin >> d;
      return TableEntry(d);
    } else if (columns[idx].column_type == EntryType::String) {
      string s;
      cin >> s;
      return TableEntry(s);
    }
    return TableEntry(0);
  }

  int find_column(string s) { // 地毯
    for (int i = 0; i < static_cast<int>(columns.size()); i++) {
      if (columns[i].column_name == s) {
        return i;
      }
    }
    return -1;
  }

  int insert(int n) { // 地毯
    data.reserve(data.size() + n);
    for (int i = 0; i < n; i++) {
      vector<TableEntry> v;
      v.reserve(columns.size()); // Here is the
      for (int j = 0; j < static_cast<int>(columns.size()); j++) {
        if (columns[j].column_type == EntryType::String) {
          string s;
          cin >> s;
          v.emplace_back(s);
        } else if (columns[j].column_type == EntryType::Double) {
          double d;
          cin >> d;
          v.emplace_back(d);
        } else if (columns[j].column_type == EntryType::Int) {
          int i;
          cin >> i;
          v.emplace_back(i);
        } else if (columns[j].column_type == EntryType::Bool) {
          bool b;
          cin >> b;
          v.emplace_back(b);
        }
      }
      data.emplace_back(v);

      if (i_column >= 0) { // update the insert index
        if (i_type == 1) {
          auto h = hash_index.find(v[i_column]);
          if (h == hash_index.end()) {
            vector<int> h_1(1, static_cast<int>(data.size()) - 1);
            hash_index.emplace(v[i_column], h_1);
          } else {
            h->second.push_back(static_cast<int>(data.size()) - 1);
          }
        } else {
          auto b = bst_index.find(v[i_column]);
          if (b == bst_index.end()) {
            vector<int> b_1(1, static_cast<int>(data.size()) - 1);
            bst_index.emplace(v[i_column], b_1);
          } else {
            b->second.push_back(static_cast<int>(data.size()) - 1);
          }
        }
      }
    }

    return data.size();
  }

  int print_all(int num, string tablename, bool quiet) { // 地毯
    vector<int> select(num);
    vector<string> match(num);
    string colname;

    for (int i = 0; i < num; i++) {
      cin >> colname;
      match[i] = colname;
      select[i] = find_column(colname);

      if ((select[i]) == -1) {
        cout << "Error during PRINT: " << colname
             << " does not name a column in " << tablename << "\n";
        return -1;
      }
    }
    if (quiet) {
      cin >> colname;
      if (colname == "ALL") {
        return data.size();
      } else {
        char OP;
        cin >> colname >> OP;
        int p_index = find_column(colname);
        if (p_index >= 0) {
          TableEntry x = read(p_index);
          int count = 0;
          if (i_column == p_index && i_type == 2) {
            // cout << "index\n\n\n";
            if (OP == '<') {
              auto bst_low = bst_index.lower_bound(x);
              for (auto it = bst_index.begin(); it != bst_low; it++) {
                count += it->second.size();
              }
            } else if (OP == '=') {
              auto bst_equal = bst_index.find(x);
              if (bst_equal != bst_index.end()) {
                count += bst_equal->second.size();
              }
            } else if (OP == '>') {
              auto bst_upper = bst_index.upper_bound(x);
              for (auto it = bst_upper; it != bst_index.end(); it++) {
                count += it->second.size();
              }
            }
          } else {
            for (auto e : data) {
              if (OP == '<') {
                if (e[p_index] < x) {
                  count++;
                }
              } else if (OP == '=') {
                if (e[p_index] == x) {
                  count++;
                }

              } else if (OP == '>') {
                if (e[p_index] > x) {
                  count++;
                }
              }
            }
          }
          return count;
        }
      }
    } else { // not p
      cin >> colname;
      if (colname == "ALL") {
        for (int i = 0; i < num; i++) {
          cout << match[i] << " ";
        }
        cout << "\n";
        for (auto d : data) {
          for (int s : select) {
            cout << d[s] << " ";
          }
          cout << "\n";
        }
        return data.size();
      } else {
        char OP;
        cin >> colname >> OP;
        int p_index = find_column(colname);
        if (p_index >= 0) {
          for (int i = 0; i < num; i++) {
            cout << match[i] << " ";
          }
          cout << "\n";
          TableEntry x = read(p_index);
          int count = 0;
          if (i_column == p_index && i_type == 2) {
            if (OP == '<') {
              auto bst_low = bst_index.lower_bound(x);
              for (auto it = bst_index.begin(); it != bst_low; it++) {
                for (auto ita : it->second) {
                  count++;
                  for (auto itb : select) {
                    cout << data[ita][itb] << " ";
                  }
                  cout << "\n";
                }
              }
            } else if (OP == '=') {
              auto bst_equal = bst_index.find(x);
              if (bst_equal != bst_index.end()) {
                for (auto ita : bst_equal->second) {
                  count++;
                  for (auto itb : select) {
                    cout << data[ita][itb] << " ";
                  }
                  cout << "\n";
                }
              }
            } else if (OP == '>') {
              auto bst_upper = bst_index.upper_bound(x);
              for (auto it = bst_upper; it != bst_index.end(); it++) {
                for (auto ita : it->second) {
                  count++;
                  for (auto itb : select) {
                    cout << data[ita][itb] << " ";
                  }
                  cout << "\n";
                }
              }
            }
          } else {
            for (auto d : data) {
              if (OP == '<') {
                if (d[p_index] < x) {
                  count++;
                  for (int d_2 : select) {
                    cout << d[d_2] << " ";
                  }
                  cout << "\n";
                }
              } else if (OP == '=') {
                if (d[p_index] == x) {
                  count++;
                  for (int d_2 : select) {
                    cout << d[d_2] << " ";
                  }
                  cout << "\n";
                }

              } else if (OP == '>') {
                if (d[p_index] > x) {
                  count++;
                  for (int d_2 : select) {
                    cout << d[d_2] << " ";
                  }
                  cout << "\n";
                }
              }
            }
          }
          return count;
        } else {
          cout << "Error during PRINT: " << colname
               << " does not name a column in " << tablename << "\n";
          return -1;
        }
      }
    }
    return -1;
  }

  int delete_row(string tablename) { // check 地毯
    string WHERE;
    string colname;
    char OP;
    cin >> WHERE >> colname >> OP;
    int d_index = find_column(colname);
    int count;

    if (d_index >= 0) {
      TableEntry y = read(d_index);

      if (OP == '<') {
        auto it = remove_if(data.begin(), data.end(), less_than(y, d_index));
        count = data.end() - it;
        if (count > 0) {
          data.erase(it, data.end());
          if (i_column >= 0) {
            if (i_type == 1) {
              regenerate();
            } else {
              regenerate_2();
            }
          }
        }
      } else if (OP == '=') {
        auto it = remove_if(data.begin(), data.end(), e(y, d_index));
        count = data.end() - it;
        if (count > 0) {
          data.erase(it, data.end());
          if (i_column >= 0) {
            if (i_type == 1) {
              regenerate();
            } else {
              regenerate_2();
            }
          }
        }
      } else if (OP == '>') {
        auto it = remove_if(data.begin(), data.end(), great_than(y, d_index));
        count = data.end() - it;
        if (count > 0) {
          data.erase(it, data.end());
          if (i_column >= 0) {
            if (i_type == 1) {
              regenerate();
            } else {
              regenerate_2();
            }
          }
        }
      }
      return count;
    } else {
      cout << "Error during DELETE: " << colname
           << " does not name a column in " << tablename << "\n";
      return -1;
    }
  }

  string generate_hash(string name) { // check 地毯
    string c_name;
    cin >> c_name;
    int c_index = find_column(c_name);
    if (c_index == -1) {
      cout << "Error during GENERATE: " << c_name
           << "does not name a column in " << name << "\n";
      return "ERROR";
    }
    if (i_column == c_index && i_type == 1) {
      return columns[i_column].column_name;
    }
    i_column = c_index;
    i_type = 1;

    hash_index.clear();
    for (int row = 0; row < static_cast<int>(data.size()); row++) {
      auto h = hash_index.find(data[row][i_column]);
      if (h == hash_index.end()) {
        vector<int> v_hash(1, row);
        hash_index.emplace(data[row][i_column], v_hash);
      } else {
        h->second.push_back(row);
      }
    }
    return columns[i_column].column_name;
  }

  void regenerate() { // check地毯
    hash_index.clear();
    for (int row = 0; row < static_cast<int>(data.size()); row++) {
      auto h = hash_index.find(data[row][i_column]);
      if (h == hash_index.end()) {
        vector<int> v_hash(1, row);
        hash_index.emplace(data[row][i_column], v_hash);
      } else {
        h->second.push_back(row);
      }
    }
  }

  string generate_bst(string name) { // check 地毯

    string c_name;
    cin >> c_name;
    int c_index = find_column(c_name);
    if (c_index == -1) {
      cout << "Error during GENERATE: " << c_name
           << "does not name a column in " << name << "\n";
      return "ERROR";
    }
    if (i_column == c_index && i_type == 2) {
      return columns[i_column].column_name;
    }
    i_column = c_index;
    i_type = 2;

    bst_index.clear();
    for (int row = 0; row < static_cast<int>(data.size()); row++) {
      auto b = bst_index.find(data[row][i_column]);
      if (b == bst_index.end()) {
        vector<int> v_bst(1, row);
        bst_index.emplace(data[row][i_column], v_bst);
      } else {
        b->second.push_back(row);
      }
    }
    return columns[i_column].column_name;
  }

  void regenerate_2() { // check 地毯
    bst_index.clear();
    for (int row = 0; row < static_cast<int>(data.size()); row++) {
      auto b = bst_index.find(data[row][i_column]);
      if (b == bst_index.end()) {
        vector<int> v_bst(1, row);
        bst_index.emplace(data[row][i_column], v_bst);
      } else {
        b->second.push_back(row);
      }
    }
  }
};
