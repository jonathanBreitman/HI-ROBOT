import 'package:hr_app/my_flutter_app_icons.dart';
import 'package:flutter/material.dart';
import 'lib.dart' as lib;
import 'Authentication.dart';
import 'package:provider/provider.dart';
import 'package:loader_overlay/loader_overlay.dart';
import 'helpful_classes.dart';
import 'package:hr_app/main.dart';
import 'SignUpPage.dart';

class SignInPageWidget extends StatefulWidget {
  @override
  _SignInPageWidgetState createState() => _SignInPageWidgetState();
}

class _SignInPageWidgetState extends State<SignInPageWidget> {
  TextEditingController passwordController = TextEditingController();
  TextEditingController emailController = TextEditingController();

  FocusNode passwordFocusNode = FocusNode();
  FocusNode emailFocusNode = FocusNode();

  bool isLoading = false;
  bool obscurePassword = true;

  final _formKey = GlobalKey<FormState>();

  @override
  void initState() {
    super.initState();

    emailFocusNode = FocusNode();
    passwordFocusNode = FocusNode();
    isLoading = false;
  }

  @override
  void dispose() {
    // Clean up the controller when the widget is disposed.
    passwordController.dispose();
    emailController.dispose();

    passwordFocusNode.dispose();
    emailFocusNode.dispose();
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

  IconButton getEyeIcon(bool passOrReenter){
    return  IconButton(
      onPressed: () => {
        SwitchPasswordObscure()
      },
      icon: obscurePassword? Icon(MyFlutterApp.eye_off, size: 16, color: Colors.white,) : Icon(MyFlutterApp.eye, size: 16,color: Colors.white,),
    );
  }

  @override
  Widget build(BuildContext context) {
    final int maxPasswordLen = 14;
    final int minPasswordLen = 6;

    return GestureDetector(
      onTap: () {
        FocusScopeNode currentFocus = FocusScope.of(context);

        if (!currentFocus.hasPrimaryFocus) {
          currentFocus.unfocus();
        }
      },
      child: AbsorbPointer(
        absorbing: this.isLoading,
        child: LoaderOverlay(
          useDefaultLoading: true,
          overlayOpacity: 0.3,
          overlayColor: Theme.of(context).primaryColor,
          child: Scaffold(
              resizeToAvoidBottomInset: false,
              appBar: generalAppBar(title: "Sign In"),
              body: Container(
                decoration: lib.gradientBG,
                width: MediaQuery.of(context).size.width,
                height: MediaQuery.of(context).size.height,
                child: Padding(
                    padding: EdgeInsets.all(10),
                    child: Column(
                      children: <Widget>[
                        SizedBox(
                            height: MediaQuery.of(context).size.height * 0.02),
                        //getTitle(),
                        Form(
                          key: _formKey,
                          child: Column(
                            children: [
                              Container(
                                width: MediaQuery.of(context).size.width * 0.65,
                                child: TextFormField(
                                  textDirection: TextDirection.ltr,
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
                                  autofocus: true,
                                  controller: emailController,
                                  focusNode: emailFocusNode,
                                  style: TextStyle(color: Colors.white),
                                  decoration: InputDecoration(
                                    hintText: 'email...',
                                    labelStyle: TextStyle(color: Colors.white30),
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
                                  textDirection: TextDirection.ltr,
                                  keyboardType: TextInputType.text,

                                  validator: (value) {
                                    if (value == null || value.isEmpty) {
                                      return '*password cannot be empty';
                                    } else if (value.length > maxPasswordLen) {
                                      return '*password cannot have more than  ' +
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

                                  //keyboardType: ,
                                  decoration: InputDecoration(
                                    hintText: 'password...',
                                    labelStyle: TextStyle(color: Colors.white30),
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
                                    suffixIcon: getEyeIcon(true),
                                  ),
                                ),
                              ),
                            ],
                          ),
                        ),
                        SizedBox(
                            height: MediaQuery.of(context).size.height * 0.02),
                        //forgot password button
                        // Container(
                        //   width: MediaQuery.of(context).size.width * 0.4,
                        //   child: TextButton(
                        //     onPressed: () {
                        //       //forgot password
                        //       ScaffoldMessenger.of(context).showSnackBar(
                        //           SnackBar(content: Text('Not Implemented yet')));
                        //     },
                        //     child: Text(
                        //       'שכחתי סיסמה',
                        //       style: TextStyle(
                        //           fontSize: 18,
                        //           color: Colors.white,
                        //           fontWeight: FontWeight.bold),
                        //     ),
                        //   ),
                        // ),
                        SizedBox(
                            height: MediaQuery.of(context).size.height * 0.02),
                        Material(
                          borderRadius: BorderRadius.circular(30.0),
                          elevation: 10,
                          shadowColor: Color(0xFF000000),
                          child: Container(
                            width: MediaQuery.of(context).size.width * 0.63,
                            decoration: BoxDecoration(
                              borderRadius: const BorderRadius.all(
                                Radius.circular(30.0),
                              ),
                            ),
                            child: TextButton(
                              onPressed: () async {
                                // sign in
                                // close keyboard if open
                                FocusScopeNode currentFocus = FocusScope.of(context);
                                if (!currentFocus.hasPrimaryFocus)
                                  currentFocus.unfocus();
                                context.loaderOverlay.show();
                                this.SwitchScreenTappabilty();
                                setState(() {});
                                if (_formKey.currentState!.validate()) {
                                  bool didSignin = await Provider.of<AppUser>(
                                      context,
                                      listen: false)
                                      .login(
                                      email: emailController.text,
                                      password: passwordController.text)
                                      .catchError((error) => print(error));


                                  setState(() {});
                                  if (didSignin) {
                                    Provider.of<AppUser>(context, listen: false)
                                        .setStatus(AuthStatus.Authenticated);
                                    context.loaderOverlay.hide();
                                    this.SwitchScreenTappabilty();
                                    Navigator.pushAndRemoveUntil<dynamic>(
                                      context,
                                      MaterialPageRoute<dynamic>(
                                        builder: (BuildContext context) =>
                                            MyHomePage(title: "HI-ROBOT"),
                                      ),
                                          (route) =>
                                      false, //if you want to disable back feature set to false
                                    );
                                  } else {
                                    context.loaderOverlay.hide();
                                    this.SwitchScreenTappabilty();
                                    ScaffoldMessenger.of(context).showSnackBar(
                                        SnackBar(
                                            content: Text(
                                                'An error occurred while signing in... please make sure that the fields are filled in correctly!')));
                                  }
                                } else {
                                  context.loaderOverlay.hide();
                                  this.SwitchScreenTappabilty();
                                  setState(() {});
                                }
                              },
                              style: ButtonStyle(
                                  padding: MaterialStateProperty.all<EdgeInsets>(
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
                                'sign in',
                                style: TextStyle(
                                  fontSize: 20,
                                ),
                              ),
                            ),
                          ),
                        ),
                        SizedBox(
                            height: MediaQuery.of(context).size.height * 0.03),
                        Material(
                          borderRadius: BorderRadius.circular(30.0),
                          elevation: 10,
                          shadowColor: Color(0xFF000000),
                          child: Container(
                            width: MediaQuery.of(context).size.width * 0.63,
                            decoration: BoxDecoration(
                              borderRadius: const BorderRadius.all(
                                Radius.circular(30.0),
                              ),
                            ),
                            child: TextButton(
                              onPressed: () {
                                Navigator.push(
                                    context,
                                    MaterialPageRoute(
                                        builder: (context) =>
                                            SignupPage()));
                              },
                              style: ButtonStyle(
                                  padding: MaterialStateProperty.all<EdgeInsets>(
                                      EdgeInsets.all(17)),
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
                                'Don\'t have an account? Sign up!',

                              ),
                            ),
                          ),
                        ),
                      ],
                    )),
              )),
        ),
      ),
    );
  }

  Container getTitle() {
    return Container(
        alignment: Alignment.center,
        padding: EdgeInsets.all(10),
        child: Text(
          'Sign In',
          style: TextStyle(
              color: Colors.white, fontWeight: FontWeight.w500, fontSize: 30),
        ));
  }
}
