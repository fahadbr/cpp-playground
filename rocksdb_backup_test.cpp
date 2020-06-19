#include <rocksdb/db.h>
#include <rocksdb/utilities/backupable_db.h>

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
using namespace rocksdb;

using DBPtr = unique_ptr<DB>;
using BEPtr = unique_ptr<BackupEngine>;

const string       DBDir       = "/tmp/rocksdb";
const string       DBBackupDir = "/tmp/rocksdb_backup";
const string       latestKey   = "latest";
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

  // CompactRangeOptions cro;
  // s = db->CompactRange(cro, &slice, &slice);
  // assert(s.ok());
}

auto DeleteAll(const DBPtr& db) -> void {
  for (auto& key : keys) {
    DBDelete(db, key);
  }

  // CompactRangeOptions cro;
  // Status              s = db->CompactRange(cro, nullptr, nullptr);
  // assert(s.ok());
}

/***************
 *  open DB  *
 ***************/
auto DBOpen() -> DBPtr {
  Options options;
  options.create_if_missing = true;

  DB*    temp;
  Status s = DB::Open(options, DBDir, &temp);
  if (!s.ok()) {
    throw runtime_error{"exception while opening db: " + s.ToString()};
  }

  DBPtr db;
  db.reset(temp);

  return db;
}

/******************************************************************************
 * Function: RestoreBackup
 *****************************************************************************/
auto RestoreBackup(int backup_id, DBPtr& db, const BEPtr& backup_engine)
    -> void {
  cout << "\nRestoring from backup: " << backup_id << "...\n";
  RestoreOptions restore_options{};

  Status s = backup_engine->RestoreDBFromBackup(backup_id,
                                                DBDir,
                                                DBDir,
                                                restore_options);

  assert(s.ok());

  db->Close();
  db = DBOpen();
  // CompactRangeOptions cro;
  // s = db->CompactRange(cro, nullptr, nullptr);
  // assert(s.ok());

  PrintKeys(db);
}

auto DoStuff() -> void {
  filesystem::remove_all(DBDir);
  filesystem::remove_all(DBBackupDir);
  DBPtr db = DBOpen();

  /******************************
   *  create the backup engine  *
   ******************************/
  BEPtr         backup_engine;
  BackupEngine* tempbk;
  Status        s = BackupEngine::Open(Env::Default(),
                                BackupableDBOptions{DBBackupDir},
                                &tempbk);
  assert(s.ok());
  backup_engine.reset(tempbk);

  /************************
   *  commit first value and take backup  *
   ************************/

  DBPut(db, 1);
  s = backup_engine->CreateNewBackup(db.get(), true);
  assert(s.ok());

  /*************************
   *  commit second value and take backup *
   *************************/

  DBDelete(db, "v1");
  DBPut(db, 2);
  s = backup_engine->CreateNewBackup(db.get(), true);
  assert(s.ok());

  DeleteAll(db);
  PrintKeys(db);

  RestoreBackup(1, db, backup_engine);
  //RestoreBackup(2, db, backup_engine);
}

