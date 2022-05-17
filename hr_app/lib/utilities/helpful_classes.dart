import 'package:hr_app/my_flutter_app_icons.dart';
import 'package:flutter/material.dart';
import 'package:hr_app/main.dart';
import 'package:intl/intl.dart';


enum JoyStickDirection{
  NONE, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
}
enum JoyStickPower{
  NONE, HALF_POWER, FULL_POWER
}

String UsersStr = "Users";

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