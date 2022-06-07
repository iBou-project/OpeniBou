var Client = require('node-rest-client').Client;

var client = new Client();

var latitude = 35.6;
var longitude = 140.1;
var key = "";
var r = 2.0;
var output_format = "json";
var lang = "ja-Jp"
var search_type = "Train%20Stations";

var url = `https://dev.virtualearth.net/REST/v1/LocationRecog/${latitude},${longitude}?key=${key}&r=${r}&output=${output_format}&type=${search_type}&c=${lang}`;
//var url = `https://dev.virtualearth.net/REST/v1/LocationRecog/${latitude},${longitude}?key=${key}&r=${r}&output=${output_format}&type=${search_type}`;

console.log(url);

/*
client.get(url, function (data, response) {
	// parsed response body as js object
	console.log(data);
	// raw response
	console.log(response);
});
*/

function get_item(json, addr) {
    if (addr.length == 0) {
        return json
    }

    const key_or_num = addr[0];

    //if ( typeof key_or_num == "string" ) {
    //}
    //
    return get_item(json[key_or_num], addr.slice(1));
}

// registering remote methods
client.registerMethod("jsonMethod", url, "GET");

client.methods.jsonMethod(function (data, response) {
	// parsed response body as js object
    //console.log(data['resourceSets'][0]['resources'][0]['businessesAtLocation']);
    console.log(get_item(data, ['resourceSets', 0, 'resources', 0, 'businessesAtLocation']));
	// raw response
	//console.log(response);
});
