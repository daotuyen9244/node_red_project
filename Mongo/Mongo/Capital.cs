using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MongoDB.Bson;

namespace Mongo {
  public class Capital {
    public string Name { get; set; }
    public string state { get; set; }
    public ObjectId Id { get; set; }
  }
}
