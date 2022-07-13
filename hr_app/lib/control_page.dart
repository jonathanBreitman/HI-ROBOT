import 'dart:typed_data';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:control_pad/control_pad.dart';
import 'utilities/helpful_classes.dart';
import 'utilities/Authentication.dart';
import 'package:provider/provider.dart';
import 'package:webview_flutter/webview_flutter.dart';


class ControlScreen extends StatefulWidget {
  final DatabaseReference _db_ref;
  Map<String, Object> _fields;
  ControlScreen({Key? key, required DatabaseReference db_ref, required Map<String, Object> fields}) : _db_ref=db_ref, _fields=fields, super(key: key);

  @override
  _ControlScreenState createState() => _ControlScreenState();
}

class _ControlScreenState extends State<ControlScreen> {
  _ControlScreenState();

  @override
  void initState() {
    super.initState();
  }



  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text("Autonomous Control",
          style: TextStyle(color: Colors.white,
          fontSize: 28),
        ),

      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: <Widget>[

            EditableSetting(settingDescription: "Number of room corners", fieldName: "corners number",
              initVal: widget._fields.containsKey("corners_number")? int.parse(widget._fields["corners_number"] as String) : 1, minVal: 1, maxVal: 10, db_ref: widget._db_ref,),
            EditableSetting(settingDescription: "Time interval between taking photos (minutes)", fieldName: "snap interval",
              initVal: widget._fields.containsKey("snap_interval") ? int.parse(widget._fields["snap_interval"] as String) : 60, minVal: 1, maxVal: 1000, db_ref: widget._db_ref,),
            EditableSetting(settingDescription: "time interval between charges (seconds)", fieldName: "charge interval",
              initVal: widget._fields.containsKey("charge_interval") ? int.parse(widget._fields["charge_interval"] as String) : 7200, minVal: 1800, maxVal: 18000, db_ref: widget._db_ref,),
          ],
        ),
      ),
    );
  }
}
