import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/cupertino.dart';

List<Color> _gradient = [
  Color.fromARGB(255, 25, 120, 8),
  Color.fromARGB(255, 25, 230, 100)
];

Color btnColor = Color.fromARGB(255, 0,128,0);
var btnBorder =
RoundedRectangleBorder(borderRadius: BorderRadius.circular(100));

final gradientBG = BoxDecoration(
    gradient: LinearGradient(
        begin: Alignment.bottomLeft,
        end: Alignment.topRight,
        colors: _gradient));

/// Forces portrait-only mode application-wide
/// Use this Mixin on the main app widget i.e. app.dart
/// Flutter's 'App' has to extend Stateless widget.
///
/// Call `super.build(context)` in the main build() method
/// to enable portrait only mode
mixin PortraitModeMixin on StatelessWidget {
  @override
  Widget build(BuildContext context) {
    _portraitModeOnly();
    return SizedBox();
  }
}

/// Forces portrait-only mode on a specific screen
/// Use this Mixin in the specific screen you want to
/// block to portrait only mode.
///
/// Call `super.build(context)` in the State's build() method
/// and `super.dispose();` in the State's dispose() method
mixin PortraitStatefulModeMixin<T extends StatefulWidget> on State<T> {
  @override
  Widget build(BuildContext context) {
    _portraitModeOnly();
    return SizedBox();
  }

  @override
  void dispose() {
    _enableRotation();
  }
}

/// blocks rotation; sets orientation to: portrait
void _portraitModeOnly() {
  SystemChrome.setPreferredOrientations([
    DeviceOrientation.portraitUp,
    DeviceOrientation.portraitDown,
  ]);
}

void _enableRotation() {
  SystemChrome.setPreferredOrientations([
    DeviceOrientation.portraitUp,
    DeviceOrientation.portraitDown,
    DeviceOrientation.landscapeLeft,
    DeviceOrientation.landscapeRight,
  ]);
}

