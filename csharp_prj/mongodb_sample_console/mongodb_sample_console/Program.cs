using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using MongoDB.Bson;
using MongoDB.Driver;
using MongoDB.Driver.Builders;
using MongoDB.Driver.GridFS;
using MongoDB.Driver.Linq;
using System.Data.SqlClient;
namespace mongodb_sample_console
{
    class Program
    {
        static void Main(string[] args)
        {
            MainAsync().Wait();

            Console.WriteLine("Press enter to exit");
            Console.ReadLine();
        }

        static async Task MainAsync()
        {

            /*var client = new MongoClient();

            IMongoDatabase db = client.GetDatabase("schoool");

            var collection = db.GetCollection<Student>("students");
            var newStudents = CreateNewStudents();

            await collection.InsertManyAsync(newStudents);*/
        }

            private static IEnumerable<Student> CreateNewStudents()
            {
                var student1 = new Student
                {
                    FirstName = "Gregor",
                    LastName = "Felix",
                    Subjects = new List<string>() { "English", "Mathematics", "Physics", "Biology" },
                    Class = "JSS 3",
                    Age = 23
                };

                var student2 = new Student
                {
                    FirstName = "Machiko",
                    LastName = "Elkberg",
                    Subjects = new List<string> { "English", "Mathematics", "Spanish" },
                    Class = "JSS 3",
                    Age = 23
                };

                var student3 = new Student
                {
                    FirstName = "Julie",
                    LastName = "Sandal",
                    Subjects = new List<string> { "English", "Mathematics", "Physics", "Chemistry" },
                    Class = "JSS 1",
                    Age = 25
                };

                var newStudents = new List<Student> { student1, student2, student3 };

                return newStudents;
            }
    }
    internal class Student
    {
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public string Class { get; set; }
        public int Age { get; set; }
        public IEnumerable<string> Subjects { get; set; }
    }
}