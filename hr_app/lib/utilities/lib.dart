import 'package:flutter/material.dart';

List<Color> _gradient = [
  Color.fromARGB(255, 25, 120, 8),
  Color.fromARGB(255, 25, 230, 100)
];

Color btnColor = Color.fromARGB(255, 10, 95, 10);
var btnBorder =
RoundedRectangleBorder(borderRadius: BorderRadius.circular(100));

final gradientBG = BoxDecoration(
    gradient: LinearGradient(
        begin: Alignment.bottomLeft,
        end: Alignment.topRight,
        colors: _gradient));

