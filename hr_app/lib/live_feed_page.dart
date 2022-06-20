import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:control_pad/control_pad.dart';
import 'utilities/helpful_classes.dart';
import 'utilities/Authentication.dart';
import 'package:provider/provider.dart';
import 'package:webview_flutter/webview_flutter.dart';



class LiveFeedScreen extends StatefulWidget {
  final DatabaseReference _db_ref;
  const LiveFeedScreen({Key? key, required DatabaseReference db_ref}) : _db_ref=db_ref, super(key: key);

  @override
  _LiveFeedScreenState createState() => _LiveFeedScreenState(dbRef: _db_ref);
}

class _LiveFeedScreenState extends State<LiveFeedScreen> {

  bool _isManual=false;
  JoyStickDirection _joystickDir=JoyStickDirection.NONE;
  JoyStickPower _joystickPower=JoyStickPower.NONE;
  DatabaseReference _dbRef;
  String _feed_url = "no_feed";//liveVideoUrl;
  Completer<WebViewController> _controller =
  Completer<WebViewController>();

  @override
  void initState() {
    super.initState();
    if (Platform.isAndroid) {
      WebView.platform = SurfaceAndroidWebView();
    }

    getUrlFromDB().then((result) {
      setFeedUrl(result);
    });
  }

  Future<String> getUrlFromDB() async{
    DatabaseReference urlRef = FirebaseDatabase.instance.ref("wirelessCar/feed_url");
    final urlSnapshot = await urlRef.once(DatabaseEventType.value);
    final url = urlSnapshot.snapshot.value?.toString() ?? 'no_feed';
    urlRef.onValue.listen((event) {
      String? data = event.snapshot.value as String;
      setFeedUrl(data);
    });
    return url;

  }

  _LiveFeedScreenState({required DatabaseReference dbRef}) : _dbRef = dbRef;

  void setFeedUrl(String new_url){
    print("new url is $new_url");
    setState(() {
      _feed_url = new_url;
      _controller.future.then((value) => value.loadUrl(new_url));
    });
  }
  void switchControlType(){
    setState(() {
      _isManual = !_isManual;
    });
  }
  void changeJoystickState(JoyStickDirection dir, JoyStickPower pow){
    print("${_joystickPower},${_joystickDir}");
    _joystickDir=dir;
    _joystickPower=pow;
  }

  JavascriptChannel _toasterJavascriptChannel(BuildContext context) {
    return JavascriptChannel(
        name: 'Toaster',
        onMessageReceived: (JavascriptMessage message) {
          ScaffoldMessenger.of(context).showSnackBar(
            SnackBar(content: Text(message.message)),
          );
        });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          // Here we take the value from the MyHomePage object that was created by
          // the App.build method, and use it to set our appbar title.
          title: Text("Live Footage",
            style: TextStyle(color: Colors.green.shade900),
          ),

        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: <Widget>[
              SizedBox(
                height: MediaQuery.of(context).size.height * 0.04,
              ),
              ClipRRect(
                borderRadius: BorderRadius.circular(20),
                child: SizedBox(
                  height: MediaQuery.of(context).size.height * 0.45,
                  width: MediaQuery.of(context).size.width * 0.85,
                  child: _feed_url != "no_feed"?
                  WebView(
                    initialUrl: this._feed_url,//'https://flutter.dev',
                    javascriptMode: JavascriptMode.unrestricted,
                    onWebViewCreated: (WebViewController webViewController) {
                      _controller.complete(webViewController);
                    },
                    onProgress: (int progress) {
                      print('WebView is loading (progress : $progress%)');
                    },
                    javascriptChannels: <JavascriptChannel>{
                      _toasterJavascriptChannel(context),
                    },
                    navigationDelegate: (NavigationRequest request) {
                      if (request.url.startsWith('https://www.youtube.com/')) {
                        print('blocking navigation to $request}');
                        return NavigationDecision.prevent;
                      }
                      print('allowing navigation to $request');
                      return NavigationDecision.navigate;
                    },
                    onPageStarted: (String url) {
                      print('Page started loading: $url');
                    },
                    onPageFinished: (String url) {
                      print('Page finished loading: $url');
                    },
                    gestureNavigationEnabled: true,
                    backgroundColor: const Color(0x00000000),
                  ):
                  Center(
                    child: Text('No feed available at the moment...',
                      style: TextStyle(fontSize: 18),
                    ),
                  ),
                  /*Image.asset(
                    'assets/images/room_irobot_pov.jpg',
                    fit: BoxFit.fill,
                  ),*/
                ),
              ),
              SizedBox(
                height: _isManual? MediaQuery.of(context).size.height * 0.02 : MediaQuery.of(context).size.height * 0.12,
              ),
              Container(
                height: _isManual? MediaQuery.of(context).size.height * 0.21 : 0,
                width: _isManual? MediaQuery.of(context).size.width * 0.5 : 0,
                child: _isManual? Column(
                    mainAxisAlignment: MainAxisAlignment.start,
                    crossAxisAlignment: CrossAxisAlignment.center,
                    children: <Widget>[
                      ///take picture button
                      IconButton(

                        icon: Icon(Icons.camera_alt_rounded),
                        onPressed: () async {
                          this._dbRef.update({'snap_pic': 1});
                          ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text('took photo!'), duration: Duration(seconds: 1),));
                        },
                        iconSize: 50,
                        splashColor: Colors.green,
                        splashRadius: 30,
                      ),
                      SizedBox(
                        height: MediaQuery.of(context).size.height * 0.02,
                      ),
                      JoystickView(
                        size: 70,
                        showArrows: false,
                        backgroundColor: Colors.green[900],
                        innerCircleColor: Colors.lightGreen,
                        /*onDirectionChanged: (double degrees,double  distance) {
                          print("${degrees.toStringAsFixed(2)},${distance.toStringAsFixed(2)}");
                        }*/
                        onDirectionChanged: (double degrees,double distance) async {
                          /*This is called whenever the joystick is moved (even slightly).
                          * the robot movement is determined based on this*/
                          //print("${degrees.toStringAsFixed(2)},${distance.toStringAsFixed(2)}");
                          JoyStickPower pow = getJSPow(distance);
                          JoyStickDirection dir = getJSDir(degrees);

                          if (pow != _joystickPower || dir != _joystickDir) {
                            changeJoystickState(dir, pow);
                          }
                          this._dbRef/*.child(await Provider.of<AppUser>(
                                context,
                                listen: false)
                                .user!.uid)*/.update({"speed": (distance*255).round(),
                              "forward": (dir == JoyStickDirection.UP_LEFT ||
                                dir == JoyStickDirection.UP_RIGHT ||
                                dir == JoyStickDirection.UP
                            ), "backwards": (dir == JoyStickDirection.DOWN_LEFT ||
                                dir == JoyStickDirection.DOWN_RIGHT ||
                                dir == JoyStickDirection.DOWN
                            ), "left": (dir ==
                                JoyStickDirection.LEFT ||
                                dir == JoyStickDirection.UP_LEFT ||
                                dir == JoyStickDirection.DOWN_LEFT
                            ), "right": (dir ==
                                JoyStickDirection.RIGHT ||
                                dir == JoyStickDirection.UP_RIGHT ||
                                dir == JoyStickDirection.DOWN_RIGHT
                            )});
                          }
                      ),
                      SizedBox(
                        height: MediaQuery.of(context).size.height * 0.03,
                      ),
                    ]
                )
                    :
                    null
              ),
              Container(
                height: MediaQuery.of(context).size.height * 0.12,
                width: MediaQuery.of(context).size.width * 0.8,

                child: ElevatedButton(onPressed: () {
                  this._dbRef.update({'state': _isManual? 1: 0});
                  switchControlType();
                },
                  child: Text(_isManual? 'Switch to Autonomous' : 'Switch to Manual',
                    style: TextStyle(fontSize: _isManual? 26: 30),
                  ),
                  style: ButtonStyle(

                  ),
                ),
              ),
            ],
          ),
        )
    );
  }
}