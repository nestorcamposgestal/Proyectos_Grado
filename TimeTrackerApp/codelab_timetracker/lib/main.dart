import 'package:codelab_timetracker/page_activities.dart';
import 'package:flutter/material.dart';
import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:easy_localization/easy_localization.dart';


void main() {
  runApp(
    EasyLocalization(
        supportedLocales: [Locale('en'), Locale('es')],
        path: 'assets/translations',
        fallbackLocale: Locale('en'),
        child: MyApp()
    ),
  );
}

class MyApp extends StatelessWidget {


  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      localizationsDelegates: context.localizationDelegates,
      supportedLocales: context.supportedLocales,
      locale: context.locale,
      title: 'TimeTracker',
      theme: ThemeData(
        primaryColor: Colors.black,
        scaffoldBackgroundColor: Colors.white,
        bottomAppBarColor: Colors.blueGrey,
        accentColor: Colors.greenAccent,
        dialogBackgroundColor: Colors.greenAccent,
        unselectedWidgetColor: Colors.greenAccent,
        textTheme: TextTheme(
            subhead: TextStyle(fontSize:20.0),
            body1:TextStyle(fontSize:20.0)),
        //fontFamily: 'Roboto'
      ),
      home: PageActivities(0),
    );
  }
}