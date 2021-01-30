import 'package:flutter/material.dart';
import 'package:codelab_timetracker/tree.dart' hide getTree;

// the old running()
import 'package:codelab_timetracker/requests.dart';
// has the new running() that sends an http request to the server

import 'PageIntervals.dart';
import 'PageSearchByTag.dart';
import 'PageSearchByTime.dart';
import 'PageSettings.dart';
import 'page_activities.dart';
import 'dart:async';

import 'package:easy_localization/easy_localization.dart';

class PageRunning extends StatefulWidget {
  int id;

  PageRunning(this.id);

  @override
  _PageRunningState createState() => _PageRunningState();
}

class _PageRunningState extends State<PageRunning> {

  int _selectedIndex = 1;
  int id;
  bool _disposed = false;
  Future<Tree> futureTree;
  Map<int, Icon> task_icons = new Map();

  Timer _timer;
  static const int periodeRefresh = 2;

  // better a multiple of periode in TimeTracker, 2 seconds

  @override
  void initState() {
    super.initState();
    id = widget.id;
    futureTree = running(0);
    _activateTimer();
  }

  void _refresh() async {
    futureTree = running(id); // to be used in build()
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
      futureTree = running(id);
      if (!_disposed)
        setState(() {});
    });
  }

  void _navigateDownActivities(int childId) {
    _timer.cancel();
    // we can not do just _refresh() because then the up arrow doesnt appear in the appbar
    Navigator.of(context)
        .push(MaterialPageRoute<void>(
      builder: (context) => PageRunning(childId),
    ))
        .then((var value) {
      _activateTimer();
      _refresh();
    });
    //https://stackoverflow.com/questions/49830553/how-to-go-back-and-refresh-the-previous-page-in-flutter?noredirect=1&lq=1
  }

  void _navigateDownIntervals(int childId) {
    _timer.cancel();
    Navigator.of(context)
        .push(MaterialPageRoute<void>(
      builder: (context) => PageIntervals(childId),
    ))
        .then((var value) {
      _activateTimer();
      _refresh();
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


  void _navigateToAdd(int childId) {
    _timer.cancel();
    Navigator.of(context)
        .pushReplacement(MaterialPageRoute<void>(
      builder: (context) => PageRunning(childId),
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
              while (Navigator.of(context).canPop()) {
                print("pop");
                Navigator.of(context).pop();
                // Navigator.popUntil(context, ModalRoute.withName('/'));
              }
              // PageRunning(0);

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

  Widget _buildRow(Activity activity, int index) {
    String strDuration =
        Duration(seconds: activity.duration)
            .toString()
            .split('.')
            .first;
    // split by '.' and taking first element of resulting list removes the microseconds part
    if (activity is Project) {
      return ListTile(
        title: Text('${activity.name}'),
        trailing: Text('$strDuration'),
        onTap: () => _navigateDownActivities(activity.id),
      );
    } else if (activity is Task) {
      Task task = activity as Task;
      // at the moment is the same, maybe changes in the future

      if (!activity.active)
        task_icons[activity.id] = Icon(Icons.play_circle_fill);
      else
        task_icons[activity.id] = Icon(Icons.stop_circle_outlined);

      Widget trailing;
      trailing = Text('$strDuration');

      return ListTile(
        leading: IconButton(
            icon: task_icons[activity.id],
            onPressed: () {
              print(task_icons);
              if ((activity as Task).active) {
                stop(activity.id);
                _refresh(); // to show immediately that task has started
                //setState(() {
                //task_icons[activity.id] = Icon(Icons.play_circle_fill);
                //}
                //);
              } else {
                start(activity.id);
                _refresh(); // to show immediately that task has stopped
                // setState(() {
                // task_icons[activity.id] = Icon(Icons.stop_circle_outlined);
                //}
                //);
              }
            }),
        title: Text('${activity.name}'),
        trailing: trailing,
        onTap: () => _navigateDownIntervals(activity.id),
        onLongPress: () {
          if ((activity as Task).active) {
            stop(activity.id);
            _refresh(); // to show immediately that task has started
          } else {
            start(activity.id);
            _refresh(); // to show immediately that task has stopped
          }
        }, // TODO start/stop counting the time for tis task
      );
    }
  }


// future with listview
  // https://medium.com/nonstopio/flutter-future-builder-with-list-view-builder-d7212314e8c9
  @override
  Widget build(BuildContext context) {
    return FutureBuilder<Tree>(
      future: futureTree,
      // this makes the tree of children, when available, go into snapshot.data
      builder: (context, snapshot) {
        // anonymous function
        if (snapshot.hasData) {
          return Scaffold(
              appBar: AppBar(
                title: Text("runningTasks").tr(),
                automaticallyImplyLeading: false,
                actions: <Widget>[],
              ),
              body: ListView.separated(
                // it's like ListView.builder() but better because it includes a separator between items
                padding: const EdgeInsets.all(16.0),
                itemCount: snapshot.data.root.children.length,
                itemBuilder: (BuildContext context, int index) =>
                    ListTileTheme(
                        child: _buildRow(snapshot.data.root.children[index], index),
                        tileColor: snapshot.data.root.children[index] is Project ?
                        (snapshot.data.root.children[index].active ? Colors.lightBlueAccent.shade700 : Colors.lightBlueAccent)
                            : (snapshot.data.root.children[index].active ? Colors.greenAccent.shade400 : Colors.greenAccent),
                        shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(15.0))),
                // TODO: Lo de las esquinas no funciona ARREGLAR
                separatorBuilder: (BuildContext context, int index) =>
                const Divider(),
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
        } else if (snapshot.hasError) {
          return Text("${snapshot.error}");
        }
        // By default, show a progress indicator
        return Container(
            height: MediaQuery
                .of(context)
                .size
                .height,
            color: Colors.white,
            child: Center(
              child: CircularProgressIndicator(),
            ));
      },
    );
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

