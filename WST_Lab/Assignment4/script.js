document.getElementById("registrationForm").addEventListener("submit", function(event) {
    // Prevent default form submission
    event.preventDefault();
    console.log("Form submission attempted");


    // Retrieve form data
    var firstName = document.getElementById("firstName").value;
    var lastName = document.getElementById("lastName").value;
    var userName = document.getElementById("userName").value;
    var email = document.getElementById("email").value;
    var state = document.getElementById("state").value;
    var country = document.getElementById("country").value;
   

 /*
    console.log("First Name:", firstName);
    console.log("Last Name:", lastName);
    console.log("User Name:", userName);
 */
    if (firstName.length < 2){
        alert("First name must be at least 2 characters long.");
        return; 
    }
    if (lastName.length < 2){
        alert("Last name must be at least 2 characters long.");
        return; 
    }
    if (userName.length < 4) {
        alert("User name must be at least 4 characters long.");
        return; 
    }
    if (!/^[a-zA-Z0-9]+$/.test(userName)) {
        alert("User name must contain only alphanumeric characters.");
        return false; 
    }
    if (state.length < 2){
        alert("State name must be at least 2 characters long.");
        return; 
    }
    if (country.length < 2){
        alert("Country name must be at least 2 characters long.");
        return; 
    }
    
    fetch("submit.php", {
        method: "POST",
        body: new FormData(this)
    })
    .then(response => response.text())
    .then(data => {
        console.log(data);
        document.getElementById("registrationForm").reset();
    })
    .catch(error => console.error("Error:", error));
});

 