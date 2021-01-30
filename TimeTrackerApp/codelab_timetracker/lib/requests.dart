import 'dart:convert' as convert;
import 'package:http/http.dart' as http;
import 'tree.dart';

final http.Client client = http.Client();
// better than http.get() if multiple requests to the same server

// If you connect the Android emulator to the webserver listening to localhost:8080
const String baseUrl = "http://10.0.2.2:8080";

// If instead you want to use a real phone, run this command in the linux terminal
//   ssh -R joans.serveousercontent.com:80:localhost:8080 serveo.net
// const String baseUrl = "https://joans.serveousercontent.com";

Future<Tree> getTree(int id) async {
  String uri = "$baseUrl/get_tree?$id";
  final response = await client.get(uri);
  // response is NOT a Future because of await but since getTree() is async,
  // execution continues (leaves this function) until response is available,
  // and then we come back here
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
    print(response.body);
    // If the server did return a 200 OK response, then parse the JSON.
    Map<String, dynamic> decoded = convert.jsonDecode(convert.utf8.decode(response.bodyBytes));
    return Tree(decoded);
  } else {
    // If the server did not return a 200 OK response, then throw an exception.
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}

Future<void> reorder(int id, int oldIndex, int newIndex) async {
  String uri = "$baseUrl/reorder?$id?$oldIndex?$newIndex";
  final response = await client.get(uri);
  // response is NOT a Future because of await but since getTree() is async,
  // execution continues (leaves this function) until response is available,
  // and then we come back here
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
    print(response.body);
    // If the server did return a 200 OK response, then parse the JSON.
  } else {
    // If the server did not return a 200 OK response, then throw an exception.
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}

Future<void> remove(int id, int index) async {
  String uri = "$baseUrl/remove?$id?$index";
  final response = await client.get(uri);
  // response is NOT a Future because of await but since getTree() is async,
  // execution continues (leaves this function) until response is available,
  // and then we come back here
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
    print(response.body);
    // If the server did return a 200 OK response, then parse the JSON.
  } else {
    // If the server did not return a 200 OK response, then throw an exception.
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}


Future<void> start(int id) async {
  String uri = "$baseUrl/start?$id";
  final response = await client.get(uri);
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
  } else {
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}

Future<void> stop(int id) async {
  String uri = "$baseUrl/stop?$id";
  final response = await client.get(uri);
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
  } else {
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}

Future<void> add(int id, String name, String type, [String tags]) async {


  String uri = "$baseUrl/add?$id?$name?$type";

  if (tags != null) {
    // tags.replaceAll(' ', '%20');
    final tags_final = tags.split(',');
    for (var i = 0; i < tags_final.length; i++) {
      uri = uri + "?" + tags_final[i];
    }
  }

  //TODO
  final response = await client.get(uri);
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
  } else {
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}


Future<Tree> running(int id) async {
  String uri = "$baseUrl/running?$id";
  final response = await client.get(uri);
  // response is NOT a Future because of await but since getTree() is async,
  // execution continues (leaves this function) until response is available,
  // and then we come back here
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
    print(response.body);
    // If the server did return a 200 OK response, then parse the JSON.
    Map<String, dynamic> decoded = convert.jsonDecode(convert.utf8.decode(response.bodyBytes));
    return Tree(decoded);
  } else {
    // If the server did not return a 200 OK response, then throw an exception.
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}


Future<Tree> searchByTime(int id, DateTime initDateTime, DateTime endDateTime) async {

  int initYear = initDateTime.year;
  int initMonth = initDateTime.month;
  int initDay = initDateTime.day;
  int initHour = initDateTime.hour;
  int initMinute = initDateTime.minute;

  int endYear = endDateTime.year;
  int endMonth = endDateTime.month;
  int endDay = endDateTime.day;
  int endHour = endDateTime.hour;
  int endMinute = endDateTime.minute;

  String uri = "$baseUrl/searchByTime?$id?$initYear?$initMonth?$initDay?$initHour?$initMinute?$endYear?$endMonth?$endDay?$endHour?$endMinute";
  final response = await client.get(uri);
  // response is NOT a Future because of await but since getTree() is async,
  // execution continues (leaves this function) until response is available,
  // and then we come back here
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
    print(response.body);
    // If the server did return a 200 OK response, then parse the JSON.
    Map<String, dynamic> decoded = convert.jsonDecode(convert.utf8.decode(response.bodyBytes));
    return Tree(decoded);
  } else {
    // If the server did not return a 200 OK response, then throw an exception.
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}


Future<Tree> searchByTag(int id, String tag) async {

  String uri = "$baseUrl/searchByTag?$id?$tag";
  final response = await client.get(uri);
  // response is NOT a Future because of await but since getTree() is async,
  // execution continues (leaves this function) until response is available,
  // and then we come back here
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
    print(response.body);
    // If the server did return a 200 OK response, then parse the JSON.
    Map<String, dynamic> decoded = convert.jsonDecode(convert.utf8.decode(response.bodyBytes));
    return Tree(decoded);
  } else {
    // If the server did not return a 200 OK response, then throw an exception.
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}


Future<int> totalTime(int id, DateTime initDateTime, DateTime endDateTime) async {

  int initYear = initDateTime.year;
  int initMonth = initDateTime.month;
  int initDay = initDateTime.day;
  int initHour = initDateTime.hour;
  int initMinute = initDateTime.minute;

  int endYear = endDateTime.year;
  int endMonth = endDateTime.month;
  int endDay = endDateTime.day;
  int endHour = endDateTime.hour;
  int endMinute = endDateTime.minute;

  String uri = "$baseUrl/totalTime?$id?$initYear?$initMonth?$initDay?$initHour?$initMinute?$endYear?$endMonth?$endDay?$endHour?$endMinute";
  final response = await client.get(uri);
  // response is NOT a Future because of await but since getTree() is async,
  // execution continues (leaves this function) until response is available,
  // and then we come back here
  if (response.statusCode == 200) {
    print("statusCode=$response.statusCode");
    print(response.body);
    // If the server did return a 200 OK response, then parse the JSON.
    Map<String, dynamic> decoded = convert.jsonDecode(response.body);
    return decoded["value"];
  } else {
    // If the server did not return a 200 OK response, then throw an exception.
    print("statusCode=$response.statusCode");
    throw Exception('Failed to get children');
  }
}