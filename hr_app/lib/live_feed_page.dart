import 'package:flutter/material.dart';

class LiveFeedScreen extends StatefulWidget {
  const LiveFeedScreen({Key key}) : super(key: key);

  @override
  _LiveFeedScreenState createState() => _LiveFeedScreenState();
}

class _LiveFeedScreenState extends State<LiveFeedScreen> {
  bool _isManual=false;
  void switchControlType(){
    setState(() {
      _isManual = !_isManual;
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
                height: MediaQuery.of(context).size.height * 0.08,
              ),
              ClipRRect(
                borderRadius: BorderRadius.circular(20),
                child: SizedBox(
                  height: MediaQuery.of(context).size.height * 0.45,
                  width: MediaQuery.of(context).size.width * 0.85,
                  child: Image.asset(
                    'assets/images/room_irobot_pov.jpg',
                    fit: BoxFit.fill,
                  ),
                ),
              ),
              SizedBox(
                height: MediaQuery.of(context).size.height * 0.12,
              ),
              Container(
                height: MediaQuery.of(context).size.height * 0.12,
                width: MediaQuery.of(context).size.width * 0.8,

                child: ElevatedButton(onPressed: () {
                  /*Navigator.push(
                      context,
                      MaterialPageRoute(
                          builder: (context) =>
                              LiveFeedScreen())
                  );*/
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
