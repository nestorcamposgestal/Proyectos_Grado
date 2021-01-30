import 'package:flutter/material.dart';
import 'package:codelab_timetracker/tree.dart' hide getTree;

// the old getTree()
import 'package:codelab_timetracker/requests.dart';
// has the new getTree() that sends an http request to the server

import 'PageAdd.dart';
import 'PageIntervals.dart';
import 'PageRunning.dart';
import 'dart:async';

import 'PageSearchByTag.dart';
import 'PageSearchByTime.dart';
import 'PageSettings.dart';
import 'package:flutter_slidable/flutter_slidable.dart';

import 'package:easy_localization/easy_localization.dart';

class PageActivities extends StatefulWidget {
  int id;

  PageActivities(this.id);

  @override
  _PageActivitiesState createState() => _PageActivitiesState();
}

class _PageActivitiesState extends State<PageActivities> {
  int _selectedIndex = 0;
  int id;
  Future<Tree> futureTree;
  Map<int, Icon> task_icons = new Map();

  bool _disposed = false;

  Timer _timer;
  static const int periodeRefresh = 4;

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

  void _navigateDownActivities(int childId) {
    _timer.cancel();
    // we can not do just _refresh() because then the up arrow doesnt appear in the appbar
    Navigator.of(context)
        .push(MaterialPageRoute<void>(
      builder: (context) => PageActivities(childId),
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


  void _pushTo(dynamic Page) {
    _timer.cancel();
    Navigator.of(context)
        .push(MaterialPageRoute<void>(
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
              PageActivities(0);
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
          default:
            break;
        }
      });
  }



// future with listview
  // https://medium.com/nonstopio/flutter-future-builder-with-list-view-builder-d7212314e8c9
  @override
  Widget build(BuildContext context) {
    final ScrollController reorderScrollController = ScrollController();
    return FutureBuilder<Tree>(
      future: futureTree,
      // this makes the tree of children, when available, go into snapshot.data
      builder: (context, snapshot) {
        // anonymous function
        if (snapshot.hasData) {
          return Scaffold(
              appBar: AppBar(
                title: snapshot.data.root.id == 0? Text('homePage').tr():Text(snapshot.data.root.name),
                // automaticallyImplyLeading: false,
                actions: <Widget>[
                  IconButton(
                    icon: Icon(Icons.add_circle),
                    onPressed: () async {
                      _showPopupMenuAdd();
                    },
                  ),
                  //TODO other actions
                ],
              ),
              body: ReorderableListView(
                scrollController: reorderScrollController,
                onReorder: (int oldIndex, int newIndex) {
                  if (newIndex > snapshot.data.root.children.length) newIndex = snapshot.data.root.children.length;
                  if (oldIndex < newIndex) newIndex -= 1;

                  if (!_disposed)
                  setState(() {

                    final Activity item = snapshot.data.root.children[oldIndex];

                    snapshot.data.root.children.removeAt(oldIndex);

                    snapshot.data.root.children.insert(newIndex, item);

                    reorder(id, oldIndex, newIndex);
                  });
                },
                children: List.generate(
                  snapshot.data.root.children.length,
                  (index) {
                    String strDuration =
                        Duration(seconds: snapshot.data.root.children[index].duration).toString().split('.').first;

                    if (snapshot.data.root.children[index] is Project) {
                      return Slidable(

                          key: UniqueKey(),

                          actionPane: SlidableDrawerActionPane(),
                    actionExtentRatio: 0.25,
                    child: Container(
                      child: ListTile(
                        tileColor: snapshot.data.root.children[index].active ? Colors.lightBlueAccent.shade700 : Colors.lightBlueAccent,
                        key: ValueKey('$index'),
                        title:
                            Text('${snapshot.data.root.children[index].name}'),
                        subtitle: snapshot.data.root.children[index].duration ==
                                0
                            ? Text("projectNotStarted",
                                textScaleFactor: 0.7).tr()
                            : (snapshot.data.root.children[index].active == true
                                ? Text("projectCurrentlyActive",
                            textScaleFactor: 0.7).tr(args: [snapshot.data.root.children[index].initialDate.toString().split('.')[0]])
                                : Text("FromTo", textScaleFactor: 0.7).tr(args:[snapshot.data.root.children[index].initialDate.toString().split('.')[0], snapshot.data.root.children[index].finalDate.toString().split('.')[0]])),

                        isThreeLine: true,
                        trailing: Text(
                            '$strDuration'),
                        onTap: () => _navigateDownActivities(
                            snapshot.data.root.children[index].id),
                      )),
                        actions: <Widget>[

                          IconSlideAction(
                            closeOnTap: false,
                            caption: 'Tags',
                            color: Colors.deepPurpleAccent,
                            icon: Icons.tag,
                            onTap: () => showDialog(
                              context: context,
                              builder: (BuildContext context) {
                                return AlertDialog(
                                  title: const Text("Tags"),
                                  content:  Text("${snapshot.data.root.children[index].tags}"),
                                );
                              },
                            )
                          )],
                          secondaryActions: <Widget>[
                            IconSlideAction(
                                closeOnTap: false,
                                caption: Text('delete').tr().data,
                                color: Colors.red,
                                icon: Icons.cancel,
                                onTap: () => remove(id, index)
                            )]
                      );

                    } else {
                      Task task = snapshot.data.root.children[index] as Task;
                      // at the moment is the same, maybe changes in the future

                      if (!snapshot.data.root.children[index].active)
                        task_icons[snapshot.data.root.children[index].id] =
                            Icon(Icons.play_circle_fill);
                      else
                        task_icons[snapshot.data.root.children[index].id] =
                            Icon(Icons.stop_circle_outlined);

                      return Slidable(

                          key: UniqueKey(),

                    actionPane: SlidableDrawerActionPane(),
                    actionExtentRatio: 0.25,
                    child: Container(
                    child: ListTile(
                        tileColor: snapshot.data.root.children[index].active ? Colors.greenAccent.shade400 : Colors.greenAccent,
                        key: Key('$index'),
                        trailing: Text(
                            '$strDuration'),
                        leading: IconButton(
                            icon: task_icons[
                                snapshot.data.root.children[index].id],
                            onPressed: () {
                              print(task_icons);
                              if ((snapshot.data.root.children[index] as Task)
                                  .active) {
                                stop(snapshot.data.root.children[index].id);
                                _refresh(); // to show immediately that task has started
                                //setState(() {
                                //task_icons[activity.id] = Icon(Icons.play_circle_fill);
                                //}
                                //);
                              } else {
                                start(snapshot.data.root.children[index].id);
                                _refresh(); // to show immediately that task has stopped
                                // setState(() {
                                // task_icons[activity.id] = Icon(Icons.stop_circle_outlined);
                                //}
                                //);
                              }
                            }),
                        title:
                            Text('${snapshot.data.root.children[index].name}'),
                        subtitle: snapshot.data.root.children[index].duration ==
                                0
                            ? Text('taskNotStarted', textScaleFactor: 0.6).tr()
                            : (snapshot.data.root.children[index].active == true
                                ? Text('taskCurrentlyActive', textScaleFactor: 0.6).tr(args: [snapshot.data.root.children[index].initialDate.toString().split('.')[0]])
                                : Text('FromTo', textScaleFactor: 0.6).tr(args: [snapshot.data.root.children[index].initialDate.toString().split('.')[0], snapshot.data.root.children[index].finalDate.toString().split('.')[0]])),
                        isThreeLine: true,
                        onTap: () => _navigateDownIntervals(
                            snapshot.data.root.children[index].id),

                      )),
                          actions: <Widget>[

                            IconSlideAction(
                                closeOnTap: false,
                                caption: 'Tags',
                                color: Colors.deepPurpleAccent,
                                icon: Icons.tag,
                                onTap: () => showDialog(
                                  context: context,
                                  builder: (BuildContext context) {
                                    return AlertDialog(
                                      title: const Text("Tags"),
                                      content:  Text("${snapshot.data.root.children[index].tags}"),
                                    );
                                  },
                                )
                            )],
                          secondaryActions: <Widget>[
                            IconSlideAction(
                                closeOnTap: false,
                                caption: 'Delete',
                                color: Colors.red,
                                icon: Icons.cancel,
                                onTap: () => remove(id, index)
                            )]
                      );
                    }
                  },
                ),
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
            height: MediaQuery.of(context).size.height,
            color: Colors.white,
            child: Center(
              child: CircularProgressIndicator(),
            ));
      },
    );
  }


  void _showPopupMenuAdd() {
    showMenu<String>(
      context: context,
      position: RelativeRect.fromLTRB(25.0, 25.0, 0.0, 0.0),
      //position where you want to show the menu on screen
      items: [
        PopupMenuItem<String>(child: const Text('newProject').tr(), value: '1'),
        PopupMenuItem<String>(child: const Text('newTask').tr(), value: '2'),
      ],
      elevation: 8.0,
    ).then<void>((String itemSelected) {
      if (itemSelected == null) return;

      if (itemSelected == "1")
        _pushTo(PageAdd(id, "Project"));
      else if (itemSelected == "2") _pushTo(PageAdd(id, "Task"));
    });
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
