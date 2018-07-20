using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_date_time
{
    class Program
    {
        static void Main(string[] args)
        {
            DateTime now = DateTime.Now;

            Console.WriteLine("Now is " + now);

            // Tạo một đối tượng DateTime (năm, tháng, ngày, giờ, phút, giây).
            DateTime aDateTime = DateTime.Now;

            // In ra các thông tin:
            Console.WriteLine("Day:{0}", aDateTime.Day);
            Console.WriteLine("Month:{0}", aDateTime.Month);
            Console.WriteLine("Year:{0}", aDateTime.Year);
            Console.WriteLine("Hour:{0}", aDateTime.Hour);
            Console.WriteLine("Minute:{0}", aDateTime.Minute);
            Console.WriteLine("Second:{0}", aDateTime.Second);
            Console.WriteLine("Millisecond:{0}", aDateTime.Millisecond);

            // Enum {Monday, Tuesday,... Sunday}
            DayOfWeek dayOfWeek = aDateTime.DayOfWeek;


            Console.WriteLine("Day of Week:{0}", dayOfWeek);


            Console.WriteLine("Day of Year: {0}", aDateTime.DayOfYear);

            // Một đối tượng chỉ mô tả thời gian (giờ phút giây,..)
            TimeSpan timeOfDay = aDateTime.TimeOfDay;

            Console.WriteLine("Time of Day:{0}", timeOfDay);

            // Quy đổi ra Ticks (1 giây = 10.000.000 Ticks)
            Console.WriteLine("Tick:{0}", aDateTime.Ticks);

            // {Local, Itc, Unspecified}
            DateTimeKind kind = aDateTime.Kind;

            Console.WriteLine("Kind:{0}", kind);
            Console.ReadLine();
        }
    }
}
