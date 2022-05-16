import 'package:hr_app/my_flutter_app_icons.dart';
import 'package:flutter/material.dart';
import 'Authentication.dart';
import 'package:hr_app/main.dart';
import 'lib.dart' as lib;
import 'helpful_classes.dart';
import 'package:provider/provider.dart';
import 'package:loader_overlay/loader_overlay.dart';

class SignupPage extends StatefulWidget {

  SignupPage();

  @override
  _SignupPageState createState() => _SignupPageState();
}

class _SignupPageState extends State<SignupPage> {
  TextEditingController usernameController = TextEditingController();
  TextEditingController emailController = TextEditingController();
  TextEditingController passwordController = TextEditingController();
  TextEditingController reenterPasswordController = TextEditingController();

  FocusNode usernameFocusNode = FocusNode();
  FocusNode emailFocusNode = FocusNode();
  FocusNode passwordFocusNode = FocusNode();
  FocusNode reenterPasswordFocusNode = FocusNode();

  bool isLoading = false;

  bool obscurePassword = false;
  bool obscureReenterPassword = false;

  final _formKey = GlobalKey<FormState>();

  @override
  void initState() {
    super.initState();

    usernameFocusNode = FocusNode();
    emailFocusNode = FocusNode();
    passwordFocusNode = FocusNode();
    reenterPasswordFocusNode = FocusNode();
  }

  @override
  void dispose() {
    // Clean up the controller when the widget is disposed.
    usernameController.dispose();
    emailController.dispose();
    passwordController.dispose();
    reenterPasswordController.dispose();
    usernameFocusNode.dispose();
    emailFocusNode.dispose();
    passwordFocusNode.dispose();
    reenterPasswordFocusNode.dispose();
    super.dispose();
  }

  void SwitchScreenTappabilty(){
    setState(() {
      this.isLoading = !this.isLoading;
    });
  }

  void SwitchPasswordObscure(){
    setState(() {
      this.obscurePassword = !this.obscurePassword;
    });
  }
  void SwitchReenterPasswordObscure(){
    setState(() {
      this.obscureReenterPassword = !this.obscureReenterPassword;
    });
  }

  IconButton getEyeIcon(bool passOrReenter){
    if (passOrReenter){
      return  IconButton(
        onPressed: () => {
          SwitchPasswordObscure()
        },
        icon: obscurePassword? Icon(MyFlutterApp.eye_off, size: 16, color: Colors.white,) : Icon(MyFlutterApp.eye, size: 16,color: Colors.white,),
      );
    }
    else{
      return  IconButton(
        onPressed: () => {
          SwitchReenterPasswordObscure()
        },
        icon: obscureReenterPassword? Icon(MyFlutterApp.eye_off, size: 16, color: Colors.white,) : Icon(MyFlutterApp.eye, size: 16,color: Colors.white,),
      );
    }


  }

  @override
  Widget build(BuildContext context) {
    final int maxUsernameLen = 14;
    final int maxPasswordLen = 14;
    final int minPasswordLen = 6;

    return GestureDetector(
      onTap: () {
        FocusScopeNode currentFocus = FocusScope.of(context);

        if (!currentFocus.hasPrimaryFocus) {
          currentFocus.unfocus();
        }
      },
      child: LoaderOverlay(
        useDefaultLoading: true,
        overlayOpacity: 0.3,
        child: Scaffold(
            resizeToAvoidBottomInset: false,
            appBar: generalAppBar(
                title: "Sign Up"
            ),
            body: Container(
              decoration: lib.gradientBG,
              width: MediaQuery.of(context).size.width,
              height: MediaQuery.of(context).size.height,
              child: Padding(
                  padding: EdgeInsets.all(10),
                  child: Column(
                    children: <Widget>[
                      Container(
                          alignment: Alignment.center,
                          padding: EdgeInsets.all(10),
                          child: Text(
                            'Sign Up',
                            style: TextStyle(
                                color: Colors.white,
                                fontWeight: FontWeight.w500,
                                fontSize: 30),
                          )),
                      Form(
                        key: _formKey,
                        child: Column(
                          children: <Widget>[
                            Container(
                              width: MediaQuery.of(context).size.width * 0.65,
                              child: TextFormField(
                                textDirection: TextDirection.ltr,
                                keyboardType: TextInputType.text,
                                validator: (value) {
                                  if (value == null || value.isEmpty) {
                                    return '*username cannot be empty';
                                  } else if (value.length > maxUsernameLen) {
                                    return '*username cannot have more than ' +
                                        maxUsernameLen.toString() +
                                        ' characters';
                                  }
                                  return null;
                                },
                                autofocus: true,
                                controller: usernameController,
                                focusNode: usernameFocusNode,
                                style: TextStyle(color: Colors.white),
                                decoration: InputDecoration(
                                  hintText: 'username...',
                                  hintStyle: TextStyle(color: Colors.white30),
                                  enabledBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  focusedBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  border: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                ),
                              ),
                            ),
                            Container(
                              width: MediaQuery.of(context).size.width * 0.65,
                              child: TextFormField(
                                keyboardType: TextInputType.emailAddress,
                                validator: (value) {
                                  if (value == null || value.isEmpty) {
                                    return '*email address cannot be empty';
                                  } else if (!RegExp(
                                      r"^[a-zA-Z0-9.a-zA-Z0-9.!#$%&'*+-/=?^_`{|}~]+@[a-zA-Z0-9]+\.[a-zA-Z]+")
                                      .hasMatch(value)) {
                                    return '*illegal email address';
                                  }
                                  return null;
                                },
                                controller: emailController,
                                focusNode: emailFocusNode,
                                style: TextStyle(color: Colors.white),
                                decoration: InputDecoration(
                                  hintText: 'email...',
                                  hintStyle: TextStyle(color: Colors.white30),
                                  enabledBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  focusedBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  border: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                ),
                              ),
                            ),
                            Container(
                              width: MediaQuery.of(context).size.width * 0.65,
                              child: TextFormField(
                                obscureText: this.obscurePassword,
                                keyboardType: TextInputType.text,
                                validator: (value) {
                                  if (value == null || value.isEmpty) {
                                    return '*password cannot be empty';
                                  } else if (value.length > maxPasswordLen) {
                                    return '*password cannot have more than ' +
                                        maxPasswordLen.toString() +
                                        ' characters';
                                  } else if (value.length < minPasswordLen) {
                                    return '*password cannot have less than ' +
                                        minPasswordLen.toString() +
                                        ' characters';
                                  }
                                  return null;
                                },
                                controller: passwordController,
                                focusNode: passwordFocusNode,
                                style: TextStyle(color: Colors.white),
                                decoration: InputDecoration(
                                  hintText: 'password...',
                                  hintStyle: TextStyle(color: Colors.white30),
                                  enabledBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  focusedBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  border: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  //suffixIcon: getEyeIcon(true),

                                ),
                              ),
                            ),
                            Container(
                              width: MediaQuery.of(context).size.width * 0.65,
                              child: TextFormField(
                                obscureText: this.obscureReenterPassword,
                                keyboardType: TextInputType.text,
                                validator: (value) {
                                  if (value != passwordController.text) {
                                    return '*passwords don\'t match';
                                  }
                                  return null;
                                },
                                controller: reenterPasswordController,
                                focusNode: reenterPasswordFocusNode,
                                style: TextStyle(color: Colors.white),
                                decoration: InputDecoration(
                                  hintText: 'reenter password...',
                                  hintStyle: TextStyle(color: Colors.white30),
                                  enabledBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  focusedBorder: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  border: UnderlineInputBorder(
                                    borderSide:
                                    BorderSide(color: Colors.white30),
                                  ),
                                  //suffixIcon: getEyeIcon(false),
                                ),
                              ),
                            ),
                            SizedBox(
                                height:
                                MediaQuery.of(context).size.height * 0.08),
                            Material(
                              borderRadius: BorderRadius.circular(30.0),
                              elevation: 10,
                              shadowColor: Color(0xFF000000),
                              child: Container(
                                width: MediaQuery.of(context).size.width * 0.7,
                                decoration: BoxDecoration(
                                  borderRadius: const BorderRadius.all(
                                    Radius.circular(30.0),
                                  ),
                                ),
                                child: TextButton(
                                  onPressed: () async {
                                    //sign up
                                    //first check that password was truly confirmed:
                                    context.loaderOverlay.show();
                                    this.SwitchScreenTappabilty();
                                    setState(() {});
                                    print("showing loading sign");
                                    if (_formKey.currentState!.validate()) {
                                      await Provider.of<AppUser>(context,
                                          listen: false)
                                          .signUp(
                                          email: emailController.text,
                                          password: passwordController.text,
                                          name: usernameController.text
                                          )
                                          .catchError((error) => print(error));
                                      Provider.of<AppUser>(context,
                                          listen: false)
                                          .setStatus(AuthStatus.Authenticated);
                                      context.loaderOverlay.hide();
                                      this.SwitchScreenTappabilty();
                                      setState(() {});
                                      Navigator.pushAndRemoveUntil<dynamic>(
                                        context,
                                        MaterialPageRoute<dynamic>(
                                          builder: (BuildContext context) =>
                                              MyHomePage(title: "HI ROBOT"),
                                        ),
                                            (route) =>
                                        false, //if you want to disable back feature set to false
                                      );
                                    } else {
                                      print("not validated sign up info");
                                      context.loaderOverlay.hide();
                                      this.SwitchScreenTappabilty();
                                      setState(() {});
                                    }
                                  },
                                  style: ButtonStyle(
                                      padding:
                                      MaterialStateProperty.all<EdgeInsets>(
                                          EdgeInsets.all(15)),
                                      foregroundColor:
                                      MaterialStateProperty.all<Color>(
                                          Colors.white),
                                      backgroundColor:
                                      MaterialStateProperty.all<Color>(
                                          lib.btnColor),
                                      shape: MaterialStateProperty.all<
                                          RoundedRectangleBorder>(
                                          RoundedRectangleBorder(
                                            borderRadius: BorderRadius.circular(30),
                                          ))),
                                  child: Text(
                                    'Sign Up',
                                    style: TextStyle(
                                      fontSize: 20,
                                    ),
                                  ),
                                ),
                              ),
                            ),
                            SizedBox(
                                height:
                                MediaQuery.of(context).size.height * 0.03),
                          ],
                        ),
                      ),
                    ],
                  )),
            )),
      ),
    );
  }
}
