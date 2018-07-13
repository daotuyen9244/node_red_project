using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;
using MongoDB.Bson;
using MongoDB.Driver;
using MongoDB.Driver.Builders;

namespace Mongo {
  class Program {
    //private static 

    static void Main(string[] args) {
      #region connect DB

      string connectionString = "Server=localhost:27017";
      Console.WriteLine("Connecting MongoDB");
      MongoClient client = new MongoClient(connectionString);
      MongoServer server = client.GetServer();
      MongoDatabase mongoDatabase = server.GetDatabase("myDB");

      Console.WriteLine("Successfully  connected to MongoDB: " + mongoDatabase.Name);
      Console.WriteLine("-----------------------------");

      #endregion

      #region Get States
      MongoCollection<BsonDocument> states = ReadState(mongoDatabase);
      Console.WriteLine("States as Bson object : ----");
      foreach (BsonDocument state in states.FindAll()) {
        Console.WriteLine(state.AsBsonDocument);
      }
      Console.WriteLine("-----------------------------");
      Console.WriteLine("States name as list : ----");
      foreach (BsonDocument state in states.FindAll()) {
        Console.WriteLine(state["Name"]);
      }
      #endregion


      #region Get States As domain classes
      MongoCollection<State> statesDomain = ReadStateAsDomain(mongoDatabase);
      Console.WriteLine("States using Domain object : ----");
      foreach (State state in statesDomain.FindAll()) {
        Console.WriteLine(state.Name);
      }     
      #endregion

      #region Get Capitals
      MongoCollection<BsonDocument> capitals = ReadCapital(mongoDatabase);
      Console.WriteLine("Capital List : ----");
      foreach (BsonDocument capital in capitals.FindAll()) {
        Console.WriteLine("capital :" + capital["Name"] + " ---- State : " + capital["State"]);
      }

      #endregion

      #region Create new State
      #endregion

      #region Update Capital
      //Console.WriteLine("Enter state name which need to be updated in Capital collection");
      //string oldName = Console.ReadLine();
      //Console.WriteLine("Enter new state name which need to be set in Capital collection");
      //string newName = Console.ReadLine();
      //MongoCollection<BsonDocument> capitals = UpdateCapital(mongoDatabase,oldName,newName);
      //  Console.WriteLine("Capital List : ----");
      //foreach (BsonDocument capital in capitals.FindAll()) {
      //  Console.WriteLine("capital :" + capital["Name"] + " ---- State : " + capital["State"]);
      //}
      #endregion

      #region Delete State
      #endregion


      Console.ReadLine();
    }

    private static MongoCollection<State> ReadStateAsDomain(MongoDatabase mongoDatabase) {
      MongoCollection<State> states = mongoDatabase.GetCollection<State>("States");      ;
      return states;
    }

    private static MongoCollection<BsonDocument> CreateState(string stateName, MongoDatabase mongoDatabase) {
      MongoCollection<BsonDocument> states = mongoDatabase.GetCollection<BsonDocument>("States");
      BsonDocument state = new BsonDocument { { "Name", stateName } };
      states.Insert(state);
      return states;
    }

    private static MongoCollection<BsonDocument> ReadState(MongoDatabase mongoDatabase) {
      MongoCollection<BsonDocument> states = mongoDatabase.GetCollection<BsonDocument>("States");      
      return states;
    }

   
    private static MongoCollection<BsonDocument> UpdateCapital(MongoDatabase mongoDatabase, string oldName,string newName) {
      MongoCollection<BsonDocument> capitals = mongoDatabase.GetCollection<BsonDocument>("Capital");     
      FindAndModifyArgs args = new FindAndModifyArgs() {
        Query = Query.EQ("State", oldName),
        Update = Update.Set("State", newName),
        Upsert = false,
        SortBy = null,
        VersionReturned = FindAndModifyDocumentVersion.Original,
      };
      FindAndModifyResult res = capitals.FindAndModify(args);      
      return capitals;
    }

    private static MongoCollection<BsonDocument> DeleteState(MongoDatabase mongoDatabase, string stateName) {
      MongoCollection<BsonDocument> capitals = mongoDatabase.GetCollection<BsonDocument>("Capital");
      var query = Query.EQ("State", stateName);
      capitals.Remove(query);
      return capitals;
    }

    private static MongoCollection<BsonDocument> ReadCapital(MongoDatabase mongoDatabase) {
      MongoCollection<BsonDocument> capitals = mongoDatabase.GetCollection<BsonDocument>("Capital");
      return capitals;
    }



    
  }
}
