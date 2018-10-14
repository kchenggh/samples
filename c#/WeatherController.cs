//  --------------------------------------------------------------
//      filename:  simple web api
//  --------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;

//	http://ip_address:port/api/Weather/GetWeather?zip=48201
namespace webapiTest.Controllers
{
	[Route("api/[controller]")]
	public class WeatherController : Controller
	{
		[HttpGet("[action]")]
		public async Task<Weather> GetWeather(string zip)
		{
			if(string.IsNullOrWhiteSpace(zip)) return null;
			zip = zip.Substring(0, 5);
			string key = "your_api_key";
			string queryString = "http://api.openweathermap.org/data/2.5/weather?zip=" + zip
				 + ",us&appid=" + key + "&units=imperial";

			//	check api access token here

			HttpClient client = new HttpClient();
			try {
				var response = await client.GetAsync(queryString);

				dynamic data = null;
				if(response != null) {
					string json = response.Content.ReadAsStringAsync().Result;
					data = JsonConvert.DeserializeObject(json);

					if(data["weather"] != null) {
						Weather weather = new Weather();
						weather.Title = (string)data["name"];
						weather.Wind = (string)data["wind"]["speed"] + " mph";
						weather.Humidity = (string)data["main"]["humidity"] + " %";
						weather.Visibility = (string)data["weather"][0]["main"];

						DateTime time = new System.DateTime(1970, 1, 1, 0, 0, 0, 0);
						DateTime sunrise = time.AddSeconds((double)data["sys"]["sunrise"]);
						DateTime sunset = time.AddSeconds((double)data["sys"]["sunset"]);
						weather.Sunrise = sunrise.ToString() + " UTC";
						weather.Sunset = sunset.ToString() + " UTC";
						return weather;
					}
				}
			} catch(Exception) { }

			return null;
		}

		public class Weather
		{
			public string Title { get; set; }
			public string Temperature { get; set; }
			public string Wind { get; set; }
			public string Humidity { get; set; }
			public string Visibility { get; set; }
			public string Sunrise { get; set; }
			public string Sunset { get; set; }

			public Weather()
			{
				this.Title = "title";
				this.Temperature = "123.1";
				this.Wind = " ";
				this.Humidity = " ";
				this.Visibility = " ";
				this.Sunrise = " ";
				this.Sunset = " ";
			}
		}		
	}
}
