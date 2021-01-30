import 'dart:async';

import 'package:codelab_timetracker/page_activities.dart';
import 'package:codelab_timetracker/requests.dart';
import 'package:codelab_timetracker/tree.dart' hide getTree;
import "package:flutter/material.dart";

import 'PageRunning.dart';
import 'PageSearchByTag.dart';
import 'PageSearchByTime.dart';
import 'PageSettings.dart';

import 'package:easy_localization/easy_localization.dart';

class PageAdd extends StatefulWidget {
  int id;
  String type;

  PageAdd(this.id, this.type);

  @override
  _PageAddState createState() => _PageAddState();
}

class _PageAddState extends State<PageAdd> {
  // var to store
  // onChanged callback
  int id;
  String type;
  int _selectedIndex = 0;
  bool _disposed = false;
  Future<Tree> futureTree;

  String name;
  String tags;

  // String text = "No Value Entered";
  Timer _timer;
  static const int periodeRefresh = 2;

  @override
  void initState() {
    super.initState();
    id = widget.id;
    type = widget.type;
    futureTree = getTree(0);
    _activateTimer();
  }

  void _refresh() async {
    futureTree = getTree(id); // to be used in build()
    if (!_disposed) setState(() {});
  }

  @override
  void dispose() {
    // "The framework calls this method when this State object will never build again"
    // therefore when going up
    _timer.cancel();
    _disposed = true;
    super.dispose();
  }

  void _activateTimer() {
    futureTree = getTree(id);
    _timer = Timer.periodic(Duration(seconds: periodeRefresh), (Timer t) {
      if (!_disposed) setState(() {});
      futureTree = getTree(id);
    });
  }

  void _navigateTo(dynamic Page) {
    _timer.cancel();
    Navigator.of(context)
        .pushReplacement(MaterialPageRoute<void>(
      builder: (context) => Page,
    ))
        .then((var value) {
      _activateTimer();
      _refresh();
    });
  }

  void _onItemTapped(int index) {
    if (!_disposed)
      setState(() {
        _selectedIndex = index;
        switch (_selectedIndex) {
          case 0:
            {
              futureTree = getTree(id);
              _navigateTo(PageActivities(id));
            }
            break;
          case 1:
            {
              futureTree = running(id);
              _navigateTo(PageRunning(id));
              //PageRunning(0);

              break;
            }
          case 2:
            {
              _showPopupMenuSearch();
              break;
            }

          case 3:
            {
              _navigateTo(PageSettings(id));

              break;
            }
          // TODO: add new task, project
          // TODO: edit task, project properties
          default:
            break;
        }
      });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: type == "Project"
              ? Text("addNewProject").tr()
              : Text("addNewTask").tr(),
          automaticallyImplyLeading: false,
          actions: <Widget>[],
        ),
        body: Column(
          children: [
            Padding(
              padding: const EdgeInsets.all(15),
              child: TextField(
                style: TextStyle(color: Colors.white),
                decoration: InputDecoration(
                    labelText: Text('Title*').tr().data,
                    labelStyle: new TextStyle(color: Colors.greenAccent),
                    enabledBorder: UnderlineInputBorder(
                        borderSide: BorderSide(color: Colors.greenAccent)),
                    focusedBorder: UnderlineInputBorder(
                        borderSide: BorderSide(color: Colors.greenAccent))),
                onChanged: (value) => name = value,
              ),
            ),
            Padding(
              padding: const EdgeInsets.all(15),
              child: TextField(
                style: TextStyle(color: Colors.white),
                decoration: InputDecoration(
                    labelText: Text("addTags").tr().data,
                    labelStyle: new TextStyle(color: Colors.greenAccent),
                    enabledBorder: UnderlineInputBorder(
                        borderSide: BorderSide(color: Colors.greenAccent)),
                    focusedBorder: UnderlineInputBorder(
                        borderSide: BorderSide(color: Colors.greenAccent))),
                onChanged: (value) => tags = value,
              ),
            ),
            SizedBox(
              height: 8,
            ),
            RaisedButton(
              onPressed: () {
                if (name != null) {
                  add(id, name, type, tags);
                  _onItemTapped(0);
                }
              },
              child: Text('Create'),
              color: Colors.greenAccent,
              splashColor: Colors.greenAccent.shade700,
              shape: RoundedRectangleBorder(
                  borderRadius: new BorderRadius.circular(18.0)
              ),
              elevation: 8,
            ),
            SizedBox(
              height: 20,
            ),
            // Text("placeholder"), //TODO
          ],
        ),
        backgroundColor: Colors.grey.shade900,
        bottomNavigationBar: BottomNavigationBar(
          type: BottomNavigationBarType.fixed,
          items: <BottomNavigationBarItem>[
            BottomNavigationBarItem(
              icon: Icon(Icons.home),
              label: 'Home',
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.play_arrow),
              label: Text('appbarRunning').tr().data,
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.search),
              label: Text('appbarSearch').tr().data,
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.settings),
              label: Text('appbarSettings').tr().data,
            ),
          ],
          currentIndex: _selectedIndex,
          backgroundColor: Colors.black,
          unselectedItemColor: Colors.grey,
          selectedItemColor: Colors.greenAccent,
          onTap: _onItemTapped,
        ));
  }

  void _showPopupMenuSearch() {
    showMenu<String>(
      context: context,
      position: RelativeRect.fromLTRB(25.0, 521.0, 0.0, 0.0),
      //position where you want to show the menu on screen
      items: [
        PopupMenuItem<String>(child: const Text('byTag').tr(), value: '1'),
        PopupMenuItem<String>(child: const Text('byPeriod').tr(), value: '2'),
      ],
      elevation: 8.0,
    ).then<void>((String itemSelected) {
      if (itemSelected == null) return;

      if (itemSelected == "1")
        _navigateTo(PageSearchByTag(id));
      else if (itemSelected == "2") _navigateTo(PageSearchByTime(id));
    });
  }
}
