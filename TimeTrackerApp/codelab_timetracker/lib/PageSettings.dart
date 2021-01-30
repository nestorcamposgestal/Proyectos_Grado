import 'package:flutter/material.dart';
import 'package:codelab_timetracker/tree.dart' hide getTree;

// the old running()
import 'package:codelab_timetracker/requests.dart';
import 'package:settings_ui/settings_ui.dart';
// has the new running() that sends an http request to the server

import 'PageIntervals.dart';
import 'PageRunning.dart';
import 'PageSearchByTag.dart';
import 'PageSearchByTime.dart';
import 'PageSettings.dart';
import 'page_activities.dart';
import 'dart:async';

import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:easy_localization/easy_localization.dart';

class PageSettings extends StatefulWidget {
  int id;

  PageSettings(this.id);

  @override
  _PageSettingsState createState() => _PageSettingsState();
}

class _PageSettingsState extends State<PageSettings> {

  int _selectedIndex = 3;
  int id;
  Future<Tree> futureTree;
  Map<int, Icon> task_icons = new Map();
  bool _disposed = false;

  bool _lights = false;

  Timer _timer;
  static const int periodeRefresh = 2;

  // better a multiple of periode in TimeTracker, 2 seconds

  @override
  void initState() {
    super.initState();
    id = widget.id;
    futureTree = getTree(0);
    _activateTimer();
  }

  void _refresh() async {
    futureTree = getTree(id); // to be used in build()
    if (!_disposed)
    setState(() {});
  }

  @override
  void dispose() {
    // "The framework calls this method when this State object will never build again"
    // therefore when going up
    _timer.cancel();
    super.dispose();
  }

  void _activateTimer() {
    _timer = Timer.periodic(Duration(seconds: periodeRefresh), (Timer t) {
      futureTree = getTree(id);
      if (!_disposed)
      setState(() {});
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
        case 0: {
          _navigateTo(PageActivities(id));
          // futureTree = getTree(id);
          // PageRunning(0);
          while (Navigator.of(context).canPop()) {
            print("pop");
            Navigator.of(context).pop();
            // Navigator.popUntil(context, ModalRoute.withName('/'));
          }
          break;
        }
        case 1:
          {
          _navigateTo(PageRunning(id));
          }

          break;

        case 2:
          {
            _showPopupMenuSearch();
            break;
          }

        case 3:
          {

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
          title: Text("appbarSettings").tr(),
          automaticallyImplyLeading: false,
          actions: <Widget>[],
        ),
        body: SettingsList(
          sections: [
            SettingsSection(
              tiles: [
                SettingsTile(
                  title: Text('language').tr().data,
                  subtitle: context.locale == Locale('en') ? 'English':'Español',
                  leading: Icon(Icons.language),
                  onPressed: (BuildContext context) {context.locale == Locale('es') ? context.locale = Locale('en'): context.locale = Locale('es');},
                ),
                SettingsTile.switchTile(
                  title: Text('darkMode').tr().data,
                  leading: Icon(Icons.lightbulb),
                  switchValue: _lights,
                  onToggle: (bool value) {_lights = false? _lights = true:_lights = false;},
                ),
              ],
            ),
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


