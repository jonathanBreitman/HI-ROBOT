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
  FirebaseAuth _auth = FirebaseAuth.instance;
  User? _user;
  AuthStatus _status = AuthStatus.Uninitialized;
  LoadingStatus loadingStatus = LoadingStatus.idle;
  String profilePickUrl =
      'https://upload.wikimedia.org/wikipedia/commons/8/89/Portrait_Placeholder.png';

  String name="";

  AppUser() {
    _user = _auth.currentUser;
    _onAuthStateChanged(_user);

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

  User? get user => _user;

  bool get isAuthenticated => _status == AuthStatus.Authenticated;

  Future<void> getPicUrl() async {
    if(_user!= null) {
      profilePickUrl = await firebase_storage.FirebaseStorage.instance
          .ref()
          .child(_user!.uid)
          .getDownloadURL()
          .catchError((error) => profilePickUrl);
    }
    else{
      profilePickUrl="";
    }
    notifyListeners();
  }

  Future<void> signUp(
      {required String email, required String password, required String name}) async {
    print("started signup");
    _auth = FirebaseAuth.instance;
    _status = AuthStatus.Authenticating;
    notifyListeners();
    await _auth
        .createUserWithEmailAndPassword(email: email, password: password)
        .catchError((error) =>
        print(
            "create user with email and password failed" + error.toString()));
    print("created user");
    _user = _auth.currentUser;
    _user!.updateDisplayName(name);
    Map<String, dynamic> data = {
      'email': email,
      'password': password,
      'name': name
    };
    await FirebaseFirestore.instance
        .collection(UsersStr)
        .doc(_user!.uid)
        .set(data)
        .then((value) => print("User Added"))
        .catchError((error) => print("Failed to add user: $error"));

    FirebaseDatabase.instance.ref('wirelessCar').set({'speed': 0, 'forward': false, 'backwards': false, 'left': false, 'right': false});
    //FirebaseDatabase.instance.ref('dir_commands/' + _user!.uid).set({'power': 0, 'forward': false, 'back': false, 'left': false, 'right': false});
    _status = AuthStatus.Authenticated;
    this.name = name;
    await getUsername();
    notifyListeners();
  }

  Future<bool> login({required String email, required String password}) async {
    print("logging in...");
    try {
      _status = AuthStatus.Authenticating;
      notifyListeners();
      await _auth.signInWithEmailAndPassword(email: email, password: password);
      _user = _auth.currentUser;
      _user!.updateDisplayName(name);
      var dbUser = await FirebaseFirestore.instance
          .collection(UsersStr)
          .doc(_auth.currentUser!.uid)
          .get();

      _status = AuthStatus.Authenticated;
      await getUsername();
      notifyListeners();
      return true;
    } catch (e) {
      _status = AuthStatus.Unauthenticated;
      _user = null;
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
        .doc(_auth.currentUser!.uid)
        .get();
    await _auth.signOut();
    _status = AuthStatus.Unauthenticated;
    _user = null;
    profilePickUrl =
    'https://upload.wikimedia.org/wikipedia/commons/8/89/Portrait_Placeholder.png';

    name = '';
    notifyListeners();
    return Future.delayed(Duration.zero);
  }

  Future<void> _onAuthStateChanged(User? firebaseUser) async {
    if (firebaseUser == null) {
      _user = null;
      _status = AuthStatus.Unauthenticated;
    } else {
      _user = firebaseUser;
      _status = AuthStatus.Authenticated;
      await getPicUrl();
    }
    notifyListeners();
  }

  Future<String> getUsername() async {
    if (_user == null) {
      throw (Exception("ERROR, USER MUST BE ACCESSIBLE"));
    } else {
      var dbUser = await FirebaseFirestore.instance
          .collection(UsersStr)
          .doc(_auth.currentUser!.uid)
          .get()
          .catchError((error) => {print(error.toString())});
      name = await dbUser.data()!['name'];
      _user!.updateDisplayName(name);
      return name;
    }
  }

  Future<DocumentSnapshot> getUserInfo() async {
    if (_user == null) {
      throw (Exception("ERROR, USER MUST BE ACCESSIBLE"));
    } else {
      return await FirebaseFirestore.instance
          .collection(UsersStr)
          .doc(_auth.currentUser!.uid)
          .get()
          .catchError((error) => {print(error.toString())});
    }
  }


  /*
  Future<DocumentSnapshot> getShowReqInfo(String showReqID) async {
    return await FirebaseFirestore.instance
        .collection(ShowRequestsStr)
        .doc(showReqID)
        .get()
        .catchError((error) => {print(error.toString())});
  }
  */

}

