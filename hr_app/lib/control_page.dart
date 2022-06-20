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
  const ControlScreen({Key? key, required DatabaseReference db_ref}) : _db_ref=db_ref, super(key: key);

  @override
  _ControlScreenState createState() => _ControlScreenState(dbRef: _db_ref);
}

class _ControlScreenState extends State<ControlScreen> {
  DatabaseReference _dbRef;
  Map<String, Object> fields = {};
  _ControlScreenState({required DatabaseReference dbRef}) : _dbRef = dbRef;

  @override
  void initState() {
    super.initState();
    getFieldsFromDB().then((db_fields){
      setState(() {
        fields = db_fields;
      });
    });
  }

  Future<Map<String, Object>> getFieldsFromDB() async{
    Map<String, Object> fields = {};
    DatabaseReference cornersRef = FirebaseDatabase.instance.ref("wirelessCar/corners_num");
    final cornerSnapshot = await cornersRef.once(DatabaseEventType.value);
    final String cornersNum = cornerSnapshot.snapshot.value?.toString() ?? '4';
    fields.putIfAbsent("corners_number", () => cornersNum);

    DatabaseReference intervalRef = FirebaseDatabase.instance.ref("wirelessCar/snap_interval");
    final intervalSnapshot = await intervalRef.once(DatabaseEventType.value);
    final String intervalTime = intervalSnapshot.snapshot.value?.toString() ?? '60';
    fields.putIfAbsent("snap_interval", () => intervalTime);

    return fields;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text("Autonomous Control",
          style: TextStyle(color: Colors.green.shade900,
          fontSize: 28),
        ),

      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: <Widget>[

            EditableSetting(settingDescription: "Number of room corners", fieldName: "corners_number",
              initVal: fields.containsKey("corners_number")? fields["corners_number"] as String : "0", db_ref: widget._db_ref,),
            EditableSetting(settingDescription: "Time interval between taking photos (minutes)", fieldName: "snap_interval",
              initVal: fields.containsKey("snap_interval") ? fields["snap_interval"] as String : "0", db_ref: widget._db_ref,)
          ],
        ),
      ),
    );
  }
}
