<?php
// Check if form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve form data and sanitize
    $firstName = isset($_POST["firstName"]) ? htmlspecialchars($_POST["firstName"]) : '';
    $lastName = isset($_POST["lastName"]) ? htmlspecialchars($_POST["lastName"]) : '';
    $userName = isset($_POST["userName"]) ? htmlspecialchars($_POST["userName"]) : '';
    $email = isset($_POST["email"]) ? htmlspecialchars($_POST["email"]) : '';
    $state = isset($_POST["state"]) ? htmlspecialchars($_POST["state"]) : '';
    $country = isset($_POST["country"]) ? htmlspecialchars($_POST["country"]) : '';

    // Connect to database
    $dbname = "assignment4";

    $conn = new mysqli("localhost", "root", "", $dbname);
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
    

    //Form validation
    $checkUsernameQuery = "SELECT * FROM user WHERE user_name = '$userName'";
    $result = $conn->query($checkUsernameQuery);
    if ($result->num_rows > 0) {
        echo "Username already exists.";
        exit(); // Exit if username exists
    }

    $checkEmailQuery = "SELECT * FROM user WHERE email = '$email'";
    $result = $conn->query($checkEmailQuery);
    if ($result->num_rows > 0) {
        echo "Email already exists.";
        exit(); // Exit PHP script if email exists
    }

    // Insert form data into database
    $sql = "INSERT INTO user (first_name, last_name, user_name, email, state, country) VALUES (?, ?, ?, ?, ?, ?)";
    
    $stmt = $conn->prepare($sql);
        
    //$temp = "temp";

    // Bind variables to the prepared statement
    $stmt->bind_param("ssssss", $firstName, $lastName, $userName, $email, $state, $country);

    if ($stmt->execute()) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $stmt->error;
    }
    
    // Close statement and database connection
    $stmt->close();
    $conn->close();
    
} else {
    echo "Form submission method not allowed.";
}
?>    