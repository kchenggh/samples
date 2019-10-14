using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace reactform.Controllers
{
    public class contactFormData
    {
        public string email { get; set; }
        public string name { get; set; }
        public string recaptcha { get; set; }
    }


    [Route("api/[controller]")]
    [ApiController]
    public class ContactUsController : ControllerBase
    {
        [HttpPost]
        public async Task<bool> Post([FromBody] contactFormData value)
        {
            var formdataa = value;

            return true;
        }
    }
}