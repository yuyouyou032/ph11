// Update CO
function CO() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("co").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/co", true);
  xhttp.send();
}

// Update CO2
function CO2() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("co2").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/co2", true);
  xhttp.send();
}

// Update NH4
function NH4() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("nh4").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/nh4", true);
  xhttp.send();
}

// Update ALC
function ALC() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("alc").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/alc", true);
  xhttp.send();
}

// Update TOL
function TOL() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("tol").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/tol", true);
  xhttp.send();
}

// Update ACT
function ACT() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("act").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/act", true);
  xhttp.send();
}