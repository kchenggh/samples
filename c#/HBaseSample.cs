//  --------------------------------------------------------------
//  Copyright(C) 2016 Kaiwen Cheng. All Rights Reserved.
//  --------------------------------------------------------------
//  Disclaimers
//  NO WARRANTY FOR SOURCE CODE
//  ALL SOURCE CODE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.HBase.Client;
using Microsoft.HBase.Client.LoadBalancing;
using org.apache.hadoop.hbase.rest.protobuf.generated;

namespace tHBase00 {
    class Program {
        static HBaseClient hbaseClient = null;
        static string hbaseTableName = "sampleHbaseTable";

        static void Main(string[] args) {
            Stopwatch dbStopWatch = new Stopwatch();
            try {
                string hbaseTableName = "sampleHbaseTable";
                var creds = new ClusterCredentials(new Uri("https://yourhdinsightsvr.azurehdinsight.net"), "user", "passwd");
                hbaseClient = new HBaseClient(creds);

                var version = hbaseClient.GetVersionAsync().GetAwaiter().GetResult();
                Console.WriteLine(version);

                // Insert data into the HBase table.
                dbStopWatch.Start();
                int[] ids = Enumerable.Range(0, 10).ToArray();
                Parallel.ForEach(ids, i => runInsertTest(i).Wait());
                dbStopWatch.Stop();
                Console.WriteLine("insert 10000 = " + dbStopWatch.Elapsed.TotalMilliseconds + " ms");

                Console.WriteLine("Press ENTER to continue ...");
                Console.ReadLine();
            } catch (Exception e) {
                Console.WriteLine(e.ToString());
            }
        }

        static async Task<int> runInsertTest(int taskID) {
            Console.WriteLine(taskID + " started");
            Random rnd = new Random();
            string testKey = "";
            int iKey = 0;
            string testValue = "";
            for (int i = 0; i < 1000; i++) {
                testValue = Guid.NewGuid().ToString();
                iKey = rnd.Next(1, 100000);
                testKey = Guid.NewGuid().ToString();
                CellSet cellSet = new CellSet();
                CellSet.Row cellSetRow = new CellSet.Row { key = Encoding.UTF8.GetBytes(testKey) };
                cellSet.rows.Add(cellSetRow);

                Cell value = new Cell { column = Encoding.UTF8.GetBytes("d:catagory"), data = Encoding.UTF8.GetBytes(testValue) };
                cellSetRow.values.Add(value);
                await hbaseClient.StoreCellsAsync(hbaseTableName, cellSet);
                if (i % 50 == 0) Console.Write(".");
            }
            return 0;
        }
    }
}
