<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "assignemnt4";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);

// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}
echo "Connected successfully";

// Run a SQL query to select data from the users table
$sql = "SELECT * FROM users";
$result = mysqli_query($conn, $sql);

// Check for errors
if (!$result) {
    die("Query failed: " . mysqli_error($conn));
}

// Fetch the result data
if (mysqli_num_rows($result) > 0) {
    while($row = mysqli_fetch_assoc($result)) {
        echo "ID: " . $row["id"]. "<br>";
        echo "First Name: " . $row["first_name"]. "<br>";
        echo "Last Name: " . $row["last_name"]. "<br>";
        echo "User Name: " . $row["user_name"]. "<br>";
        echo "Email: " . $row["email"]. "<br>";
        echo "State: " . $row["state"]. "<br>";
        echo "Country: " . $row["country"]. "<br>";
        echo "Created At: " . $row["created_at"]. "<br>";
        echo "<hr>";
    }
} else {
    echo "0 results";
}

// Close the connection
mysqli_close($conn);
?>
