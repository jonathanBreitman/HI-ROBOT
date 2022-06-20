import 'package:flutter/services.dart';
import 'package:hr_app/my_flutter_app_icons.dart';
import 'package:flutter/material.dart';
import 'package:hr_app/main.dart';
import 'package:intl/intl.dart';
import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';



enum JoyStickDirection{
  NONE, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
}
enum JoyStickPower{
  NONE, HALF_POWER, FULL_POWER
}

String UsersStr = "Users";

String liveVideoUrl = 'https://www.youtube.com/watch?v=hq5dvgK5roM';

JoyStickDirection getJSDir(double degrees){
  JoyStickDirection dir = JoyStickDirection.NONE;
  if(degrees>0.001 && degrees < 359.99) {
    if (degrees >= 360 - 22.5 || degrees < 22.5) {
      dir = JoyStickDirection.UP;
    }
    else if (degrees >= 22.5 && degrees < 45+22.5) {
      dir = JoyStickDirection.UP_RIGHT;
    }
    else if (degrees >= 45+22.5 && degrees < 90+22.5) {
      dir = JoyStickDirection.RIGHT;
    }
    else if (degrees >= 90+22.5 && degrees < 135+22.5) {
      dir = JoyStickDirection.DOWN_RIGHT;
    }
    else if (degrees >= 135+22.5 && degrees < 180+22.5) {
      dir = JoyStickDirection.DOWN;
    }
    else if (degrees >= 180+22.5 && degrees < 225+22.5) {
      dir = JoyStickDirection.DOWN_LEFT;
    }
    else if (degrees >= 225+22.5 && degrees < 270+22.5) {
      dir = JoyStickDirection.LEFT;
    }
    else if (degrees >= 270+22.5 && degrees < 315+22.5) {
      dir = JoyStickDirection.UP_LEFT;
    }
    else{
      //should never get here
      print("IMPOSSIBLE ANGLE!!!!\n\n\n");
    }
  }
  return dir;
}

String getCleanDateStr(DateTime date){
  return DateFormat.yMMMMd('en_US').format(date) + ", " + DateFormat.Hms().format(date);
}

JoyStickPower getJSPow(double distance){
  JoyStickPower pow = JoyStickPower.NONE;
  if (distance>0.66){
    pow = JoyStickPower.FULL_POWER;
  }
  else if(distance>0.2){
    pow = JoyStickPower.HALF_POWER;
  }
  return pow;
}

class generalAppBar extends StatefulWidget implements PreferredSizeWidget {
  final String title;

  generalAppBar({this.title=""}): preferredSize = Size.fromHeight(kToolbarHeight);

  @override
  final Size preferredSize; // default is 56.0

  @override
  _generalAppBarState createState() => _generalAppBarState();
}

class _generalAppBarState extends State<generalAppBar> {
  @override
  Widget build(BuildContext context) {
    return AppBar(
      leading: Container(
        child: IconButton(
          onPressed: (){
            Navigator.of(context).pop();
            FocusScopeNode currentFocus = FocusScope.of(context);
            if (!currentFocus.hasPrimaryFocus)
              currentFocus.unfocus();
          },
          icon: Icon(Icons.arrow_back, color: Colors.white),
        ),
      ),
      actions: <Widget>[
        IconButton(
          icon: Icon(
            Icons.home,
            color: Colors.white,
          ),
          onPressed: () {
            // GO TO HOME PAGE
            Navigator.pushAndRemoveUntil<dynamic>(
              context,
              MaterialPageRoute<dynamic>(
                builder: (BuildContext context) =>
                    MyHomePage(title: "HI ROBOT"),
              ),
                  (route) =>
              false, //if you want to disable back feature set to false
            );
          },
        )
      ],
      title: Text(widget.title, style: TextStyle(color: Colors.white)),
    );
  }
}

class EditableSetting extends StatefulWidget {
  final String settingDescription;
  final String fieldName;
  final String initVal;
  final DatabaseReference _db_ref;
  const EditableSetting({Key? key, this.settingDescription="", this.fieldName="", this.initVal="", required DatabaseReference db_ref}) : _db_ref=db_ref, super(key: key);

  @override
  _EditableSettingState createState() => _EditableSettingState();
}

class _EditableSettingState extends State<EditableSetting> {
  @override
  Widget build(BuildContext context) {
    return Container(

      padding: const EdgeInsets.only(left: 10.0, right: 5.0),
      height: MediaQuery.of(context).size.height*0.08,
      child: Row(
        mainAxisAlignment: MainAxisAlignment.start,
        children: <Widget>[
          SizedBox(
            width: MediaQuery.of(context).size.width * 0.7,
            height: MediaQuery.of(context).size.height*0.08,
            child: TextFormField(
              initialValue: widget.initVal,
              decoration: InputDecoration(labelText: widget.settingDescription + ": ",
              ),
              keyboardType: TextInputType.number,
              inputFormatters: <TextInputFormatter>[
                FilteringTextInputFormatter.digitsOnly
              ],
              onChanged: (String val){
                widget._db_ref.update({widget.fieldName: int.parse(val)});
              },//
              // Only numbers can be
            ),
          ),
        ],
      ),
    );
  }
}
