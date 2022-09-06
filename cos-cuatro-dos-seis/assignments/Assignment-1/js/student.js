var Student = {
  // please fill in your name and NetID
  //  your NetID is the part of your email before @princeton.edu
  name: "Atsuko Shimizu",
  netID: "atskae", // The "Hallo Welt!" of the assignment :o)
};

Student.updateHTML = function() {
  var studentInfo = this.name + " &lt;" + this.netID + "&gt;";
  document.getElementById("student").innerHTML = studentInfo;
};
