import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/cupertino.dart';
import 'package:hr_app/saved_footage_page.dart';
import 'package:provider/provider.dart';
import 'live_feed_page.dart';
import 'utilities/SignInPage.dart';
import 'utilities/Authentication.dart';
import 'control_page.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();

  await Firebase.initializeApp();
  runApp(ChangeNotifierProvider(create: (_) => AppUser(), child: MyApp()));
  //runApp(MyApp());

}


class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'HI-ROBOT Control',
      theme: ThemeData(
        // Define the default brightness and colors.
        brightness: Brightness.light,
        primaryColor: Colors.green[900],
        backgroundColor: Colors.greenAccent[200],
        // Define the default font family.
        fontFamily: 'Georgia',

        // Define the default `TextTheme`. Use this to specify the default
        // text styling for headlines, titles, bodies of text, and more.
        textTheme: const TextTheme(
          headline1: TextStyle(fontSize: 72.0, fontWeight: FontWeight.bold, color: Colors.green),
          headline6: TextStyle(fontSize: 36.0, fontStyle: FontStyle.italic, color: Colors.green),
          bodyText2: TextStyle(fontSize: 14.0, fontFamily: 'Hind'),
        ),
        primarySwatch: Colors.green,
        scaffoldBackgroundColor: Colors.green.shade200,
      ),
      home: MyHomePage(title: 'HR - Main Menu'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, this.title: ""}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;
  late DatabaseReference _dbref;
  void _incrementCounter() {
    setState(() {
      // This call to setState tells the Flutter framework that something has
      // changed in this State, which causes it to rerun the build method below
      // so that the display can reflect the updated values. If we changed
      // _counter without calling setState(), then the build method would not be
      // called again, and so nothing would appear to happen.
      _counter++;
    });
  }

  @override
  void initState(){
    super.initState();
    _dbref = FirebaseDatabase.instance.ref('wirelessCar');
  }

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.

    final _appUser = Provider.of<AppUser>(context);
    //bool isAnyoneLoggedIn = _appUser.status == AuthStatus.Authenticated;
    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(widget.title,
          style: TextStyle(color: Colors.green.shade900),
        ),
        actions: <Widget>[
          IconButton(
            icon: Icon( !_appUser.isAuthenticated?
              Icons.account_circle : Icons.logout,
              color: Colors.white,
            ),
            onPressed: () async {
              if (!_appUser.isAuthenticated) {
                // GO TO SIGN IN/UP PAGE
                Navigator.push(
                    context,
                    MaterialPageRoute(
                        builder: (context) =>
                            SignInPageWidget())
                );
              }
              else{
                await _appUser.signOut();
              }
            },
          )
        ],
      ),
      body: Center(
        // Center is a layout widget. It takes a single child and positions it
        // in the middle of the parent.
        child: Column(
          // Column is also a layout widget. It takes a list of children and
          // arranges them vertically. By default, it sizes itself to fit its
          // children horizontally, and tries to be as tall as its parent.
          //
          // Invoke "debug painting" (press "p" in the console, choose the
          // "Toggle Debug Paint" action from the Flutter Inspector in Android
          // Studio, or the "Toggle Debug Paint" command in Visual Studio Code)
          // to see the wireframe for each widget.
          //
          // Column has various properties to control how it sizes itself and
          // how it positions its children. Here we use mainAxisAlignment to
          // center the children vertically; the main axis here is the vertical
          // axis because Columns are vertical (the cross axis would be
          // horizontal).
          mainAxisAlignment: MainAxisAlignment.start,
          children: <Widget>[
            ///title text
            Container(
              height: MediaQuery.of(context).size.height * 0.12,
              width: MediaQuery.of(context).size.width * 0.8,
              alignment: Alignment.center,
              child: Text(_appUser.isAuthenticated? "Hello, " + _appUser.user!.displayName! : "Welcome!",
                style: TextStyle(fontSize: 34),
              ),
            ),
            SizedBox(
              height: MediaQuery.of(context).size.height * 0.15,
            ),
            ///live video button
            Container(
              height: MediaQuery.of(context).size.height * 0.08,
              width: MediaQuery.of(context).size.width * 0.8,

              child: ElevatedButton(onPressed: () {
                if(!_appUser.isAuthenticated){
                  ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text('you have to sign in first!'), duration: Duration(seconds: 1),));
                }
                else {
                  if (_appUser.user!.email == 'jonathanbreitman@gmail.com') {
                    Navigator.push(
                        context,
                        MaterialPageRoute(
                            builder: (context) =>
                                LiveFeedScreen(db_ref: this._dbref))
                    );
                  }
                  else {
                    ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(content: Text(_appUser.user!.displayName! +
                            ' has no connected car...'),
                          duration: Duration(seconds: 1),));
                  }
                }
              },
                child: Text(
                    'Live Video!',
                  style: TextStyle(fontSize: 30),
                  ),
                style: ButtonStyle(

                ),
              ),
            ),
            SizedBox(
              height: MediaQuery.of(context).size.height * 0.04,
            ),
            Container(
              height: MediaQuery.of(context).size.height * 0.06,
              width: MediaQuery.of(context).size.width * 0.6,

              child: ElevatedButton(onPressed: () {
                if(!_appUser.isAuthenticated){
                  ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text('you have to sign in first!'), duration: Duration(seconds: 1),));
                }
                else {
                  if (_appUser.user!.email == 'jonathanbreitman@gmail.com') {
                    Navigator.push(
                        context,
                        MaterialPageRoute(
                            builder: (context) =>
                                ControlScreen(db_ref: this._dbref))
                    );
                  }
                  else {
                    ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(content: Text(_appUser.user!.displayName! +
                            ' has no connected car...'),
                          duration: Duration(seconds: 1),));
                  }
                }
              },
                child: Text(
                  'Control',
                  style: TextStyle(fontSize: 24),
                ),
                style: ButtonStyle(

                ),
              ),
            ),
            SizedBox(
              height: MediaQuery.of(context).size.height * 0.03,
            ),
            ///saved data button
            Container(
              height: MediaQuery.of(context).size.height * 0.06,
              width: MediaQuery.of(context).size.width * 0.6,

              child: ElevatedButton(onPressed: () {
                if(!_appUser.isAuthenticated){
                ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text('you have to sign in first!'), duration: Duration(seconds: 1),));
                }
                else {
                  if (_appUser.user!.email == 'jonathanbreitman@gmail.com') {
                    Navigator.push(
                        context,
                        MaterialPageRoute(
                            builder: (context) =>
                                SavedFootagePage())
                    );
                  }
                else {
                  ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(content: Text(_appUser.user!.displayName! +
                  ' has no connected car...'),
                  duration: Duration(seconds: 1),));
                  }
                }
              },
                child: Text(
                  'Saved Footage',
                  style: TextStyle(fontSize: 24),
                ),
                style: ButtonStyle(

                ),
              ),
            ),
            SizedBox(
              height: MediaQuery.of(context).size.height * 0.03,
            ),

          ],
        ),
      ),
    );
  }
}
