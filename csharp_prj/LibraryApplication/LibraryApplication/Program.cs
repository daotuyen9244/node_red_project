using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Driver;
using MongoDB.Driver.Linq;

namespace LibraryApplication
{
    class Program
    {
        static void Main(string[] args)
        {
            MongoClient client = new MongoClient();
            var server = client.GetServer();
            var db = server.GetDatabase("LibraryDB");
            var collection = db.GetCollection<BookStore>("BookStore");

            List<BookStore> bookStores = new List<BookStore>
            {
                new BookStore
                {
                    BookTitle = "MongoDB Basics",
                    ISBN = "8767687689898yu",
                    Auther = "Tanya",
                    Category = "NoSQL DBMS",
                    Price = 456
                },
                new BookStore
                {
                    BookTitle = "C# Basics",
                    ISBN = "27758987689898yu",
                    Auther = "Tanvi",
                    Category = "Programming Languages",
                    TotalPages = 376,
                    Price = 289
                },
                new BookStore
                {
                    BookTitle = "SQL Server Basics",
                    ISBN = "117675787689898yu",
                    Auther = "Tushar",
                    Category = "RDBMS",
                    TotalPages = 250,
                    Price = 478
                },
                new BookStore
                {
                    BookTitle = "Entity Framework Basics",
                    ISBN = "6779799933389898yu",
                    Auther = "Somya",
                    Category = "ORM tool",
                    TotalPages = 175,
                    Price = 289
                }
            };

            foreach (BookStore bookStore in bookStores)
            {
                collection.Save(bookStore);
            }

            //Query data
            var bookCount = collection.AsQueryable().Where(b => b.TotalPages > 200);
            Console.WriteLine("\n\nBooks count having pages more than 200 is => " + bookCount.Count());

            var book = collection.AsQueryable().Where(b => b.BookTitle.StartsWith("Mongo"));
            Console.WriteLine("\n\nBook whose title starts with 'Mongo' is => " + book.First().BookTitle);

            var cheapestBook = collection.AsQueryable().OrderBy(b => b.Price).First();
            Console.WriteLine("\n\nCheapest Book is => " + cheapestBook.BookTitle);

            var bookWithISBN = collection.AsQueryable().Single(b => b.ISBN == "6779799933389898yu");
            Console.WriteLine("\n\nBook with ISBN number 6779799933389898yu is => " + bookWithISBN.BookTitle);

            //collection.RemoveAll();
            Console.ReadLine();
                        
        }

        public class BookStore
        {
            //public ObjectId Id { get; set; }

            [BsonId]
            public string ISBN { get; set; }
            public string BookTitle { get; set; }
            public string Auther { get; set; }
            public string Category { get; set; }
            public decimal Price { get; set; }

            [BsonIgnoreIfNull]
            public int? TotalPages { get; set; }

        }
    }
}
