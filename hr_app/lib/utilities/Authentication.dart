import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:hr_app/utilities/helpful_classes.dart';
import 'package:image_picker/image_picker.dart';
import 'package:firebase_storage/firebase_storage.dart' as firebase_storage;
import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_database/firebase_database.dart';



enum AuthStatus {
  Uninitialized,
  Authenticated,
  Authenticating,
  Unauthenticated,
  SigningOut
}

enum LoadingStatus {
  idle, loading
}

class AppUser with ChangeNotifier {
  FirebaseAuth auth = FirebaseAuth.instance;
  AuthStatus _status = AuthStatus.Uninitialized;
  LoadingStatus loadingStatus = LoadingStatus.idle;
  String profilePickUrl =
      'https://upload.wikimedia.org/wikipedia/commons/8/89/Portrait_Placeholder.png';

  String name="";

  AppUser() {
    auth = FirebaseAuth.instance;
    _onAuthStateChanged(auth.currentUser);

  }

  AuthStatus get status => _status;

  void setStatus(stat) {
    this._status = stat;
    notifyListeners();
  }

  void setLoadingStatus(s) {
    this.loadingStatus = s;
    notifyListeners();
  }


  bool get isAuthenticated => _status == AuthStatus.Authenticated;

  Future<void> getPicUrl() async {
    if(auth.currentUser!= null) {
      profilePickUrl = await firebase_storage.FirebaseStorage.instance
          .ref()
          .child(auth.currentUser!.uid)
          .getDownloadURL()
          .catchError((error) => profilePickUrl);
    }
    else{
      profilePickUrl="";
    }
    notifyListeners();
  }

  Future<bool> signUp(
      {required String email, required String password, required String name}) async {
    print("started signup");
    auth = FirebaseAuth.instance;
    _status = AuthStatus.Authenticating;
    notifyListeners();
    try {
      await auth
          .createUserWithEmailAndPassword(email: email, password: password);
      auth = FirebaseAuth.instance;
      print("created user");
      auth.currentUser!.updateDisplayName(name);
      Map<String, dynamic> data = {
        'email': email,
        'password': password,
        'name': name
      };
      await FirebaseFirestore.instance
          .collection(UsersStr)
          .doc(auth.currentUser!.uid)
          .set(data)
          .then((value) => print("User Added"))
          .catchError((error) => print("Failed to add user: $error"));

      FirebaseDatabase.instance.ref('wirelessCar').update({
        'speed': 0,
        'forward': false,
        'backwards': false,
        'left': false,
        'right': false,
        'charge_interval': 1,
        'charge_error': 0,
        'charge_forward_delay': 1000,
        'charge_back': 800,
        'charging_time': 20,
        'corners_number': 4,
        'currently_charging': 0,
        'feed_url': "",
        'go_charge': 0,
        'low_battery': 0,
        'snap_interval': 5,
        'snap_pic': 0,
        'state': 0,

      });
      _status = AuthStatus.Authenticated;
      this.name = name;
      await getUsername();
      notifyListeners();
      print("current user: " + auth.currentUser.toString());
      return true;
    }
    catch(e){
      _status = AuthStatus.Unauthenticated;
      print("signup fail: ");
      print(e);
      notifyListeners();
      return false;
    }
  }

  Future<bool> login({required String email, required String password}) async {
    print("logging in...");
    try {
      _status = AuthStatus.Authenticating;
      notifyListeners();
      await auth.signInWithEmailAndPassword(email: email, password: password);
      auth = FirebaseAuth.instance;
      auth.currentUser!.updateDisplayName(name);
      var dbUser = await FirebaseFirestore.instance
          .collection(UsersStr)
          .doc(auth.currentUser!.uid)
          .get();

      _status = AuthStatus.Authenticated;
      await getUsername();
      notifyListeners();
      print("current user: " + auth.currentUser.toString());
      return true;
    } catch (e) {
      _status = AuthStatus.Unauthenticated;
      print("login fail: ");
      print(e);
      notifyListeners();
      return false;
    }
  }

  Future signOut() async {
    _status = AuthStatus.SigningOut;
    notifyListeners();
    var dbUser = await FirebaseFirestore.instance
        .collection(UsersStr)
        .doc(auth.currentUser!.uid)
        .get();
    await auth.signOut();
    _status = AuthStatus.Unauthenticated;
    profilePickUrl =
    'https://upload.wikimedia.org/wikipedia/commons/8/89/Portrait_Placeholder.png';

    name = '';
    notifyListeners();
    return Future.delayed(Duration.zero);
  }

  Future<void> _onAuthStateChanged(User? firebaseUser) async {
    if (firebaseUser == null) {
      _status = AuthStatus.Unauthenticated;
    } else {
      _status = AuthStatus.Authenticated;
      //await getPicUrl();
    }
    notifyListeners();
  }

  Future<String> getUsername() async {
    if (auth.currentUser == null) {
      throw (Exception("ERROR, USER MUST BE ACCESSIBLE"));
    } else {
      var dbUser = await FirebaseFirestore.instance
          .collection(UsersStr)
          .doc(auth.currentUser!.uid)
          .get()
          .catchError((error) => {print(error.toString())});
      name = await dbUser.data()!['name'];
      print("found user name: " + name);
      await auth.currentUser!.updateDisplayName(name);
      print("display name: " + (auth.currentUser!.displayName ?? "not available"));
      return name;
    }
  }

  Future<DocumentSnapshot> getUserInfo() async {
    if (auth.currentUser == null) {
      throw (Exception("ERROR, USER MUST BE ACCESSIBLE"));
    } else {
      return await FirebaseFirestore.instance
          .collection(UsersStr)
          .doc(auth.currentUser!.uid)
          .get()
          .catchError((error) => {print(error.toString())});
    }
  }


}

