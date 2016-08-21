//  --------------------------------------------------------------
//  Copyright(C) 2016 Kaiwen Cheng. All Rights Reserved.
//  --------------------------------------------------------------
//  Disclaimers
//  NO WARRANTY FOR SOURCE CODE
//  ALL SOURCE CODE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.

using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using System.Threading.Tasks;
using System.Net.Http;
using System.Collections.Generic;

namespace TestApp {
    [Activity(Label = "TestApp", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity {
        protected override void OnCreate(Bundle bundle) {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            // Get our button from the layout resource,
            // and attach an event to it
            Button button = FindViewById<Button>(Resource.Id.MyButton);

            button.Click += async (sender, e) => {
                await onClick();
            };
        }

        async Task<bool> onClick() {
            string url = "https://your_api_url";
            Button button = FindViewById<Button>(Resource.Id.MyButton);

            var values = new List<KeyValuePair<string, string>> {
                        new KeyValuePair<string, string>("clientVersion", "1"),
                        new KeyValuePair<string, string>("appID", "ID"),
                        new KeyValuePair<string, string>("appKey", "Key"),
                    };
            try {
                string responseString = "";
                var httpClient = new HttpClient();
                httpClient.DefaultRequestHeaders.Add("Accept-Language", "en-US,en");
                HttpResponseMessage response = await httpClient.PostAsync(url, new FormUrlEncodedContent(values));
                responseString = await response.Content.ReadAsStringAsync();
                button.Text = string.Format("Login Ok");
            } catch (Exception e) {
                button.Text = string.Format("Login failed");
                Console.WriteLine(e.ToString());
            }

            return true;
        }
    }
}

