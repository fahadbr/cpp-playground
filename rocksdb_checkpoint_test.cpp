#include <rocksdb/db.h>
#include <rocksdb/utilities/checkpoint.h>

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
using namespace rocksdb;

using DBPtr = unique_ptr<DB>;
using CPPtr = unique_ptr<Checkpoint>;

const string       DBDir           = "/tmp/rocksdb";
const string       DBCheckpointDir = "/tmp/rocksdb_checkpoint";
const string       latestKey       = "latest";
const WriteOptions wo{};
const ReadOptions  ro{};

set<string> keys{};

auto DoStuff() -> void;

int main(void) {
  try {
    DoStuff();
  } catch (exception& ex) {
    cerr << "caught exception: " << ex.what();
  } catch (...) {
    cerr << "caught unknown exception.. exiting";
  }

  return 0;
}

auto MyValueData(uint32_t i) -> string {
  return "my value data" + to_string(i);
}

auto DBGet(const DBPtr& db, const string& key) -> string {
  string ret_val;
  Status s = db->Get(ro, key, &ret_val);
  if (!s.ok()) {
    if (s.IsNotFound()) {
      return "";
    } else {
      throw runtime_error("got some other read error");
    }
  }
  return ret_val;
}

auto PrintKeys(const DBPtr& db) -> void {
  for (auto& key : keys) {
    string val = DBGet(db, key);
    cout << key << ": " << val << '\n';
  }
}

auto DBPut(const DBPtr& db, const uint32_t index) -> void {
  string val = MyValueData(index);
  string key = 'v' + to_string(index);

  db->Put(wo, latestKey, val);
  db->Put(wo, key, val);
  keys.insert(latestKey);
  keys.insert(key);

  string latest = DBGet(db, latestKey);
  cout << "latest is: " << latest << endl;
  assert(latest == val);
}

auto DBDelete(const DBPtr& db, const string& key) -> void {
  auto s = db->Delete(wo, key);
  assert(s.ok());

  Slice slice = key;

  CompactRangeOptions cro;
  s = db->CompactRange(cro, &slice, &slice);
  assert(s.ok());
}

auto DeleteAll(const DBPtr& db) -> void {
  for (auto& key : keys) {
    DBDelete(db, key);
  }

  CompactRangeOptions cro;
  Status              s = db->CompactRange(cro, nullptr, nullptr);
  assert(s.ok());
}

/***************
 *  open DB  *
 ***************/
auto DBOpen(const string& path) -> DBPtr {
  Options options;
  options.create_if_missing = true;

  DB*    temp;
  Status s = DB::Open(options, path, &temp);
  if (!s.ok()) {
    throw runtime_error{"exception while opening db: " + s.ToString()};
  }

  DBPtr db;
  db.reset(temp);

  return db;
}

auto DoStuff() -> void {
  filesystem::remove_all(DBDir);
  filesystem::remove_all(DBCheckpointDir);

  DBPtr db = DBOpen(DBDir);

  CPPtr       cp;
  Checkpoint* temp;
  Status      s = Checkpoint::Create(db.get(), &temp);
  assert(s.ok());
  cp.reset(temp);

  DBPut(db, 1);
  s = cp->CreateCheckpoint(DBCheckpointDir);

  DBPut(db, 2);
  DBDelete(db, "v1");

  cout << "\nprinting keys from " << DBDir << '\n';
  PrintKeys(db);

  DBPtr checkpoint_db = DBOpen(DBCheckpointDir);
  cout << "\nprinting keys from " << DBCheckpointDir << '\n';
  PrintKeys(checkpoint_db);
}

