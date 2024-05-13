<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>User Profiles</title>
    <link rel="stylesheet" href="styles1.css">
</head>
<body>
    <div class="container">
        <h2>User Profiles</h2>
        <div class="profile-list">
            <?php
            // Database connection
            $servername = "localhost";
            $username = "root"; // Replace with your MySQL username
            $password = "";     // Replace with your MySQL password
            $dbname = "assignment4"; // Replace with your database name

            $conn = new mysqli($servername, $username, $password, $dbname);

            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            }

            // Fetch profiles from the database
            $sql = "SELECT * FROM user";
            $result = $conn->query($sql);

            if ($result->num_rows > 0) {
                // Output data of each row
                while($row = $result->fetch_assoc()) {
                    echo "<div class='card'>";
                    echo "<h3>" . $row['first_name'] . " " . $row['last_name'] . "</h3>";
                    echo "<p><strong>Username:</strong> " . $row['user_name'] . "</p>";
                    echo "<p><strong>Email:</strong> " . $row['email'] . "</p>";
                    echo "<p><strong>State:</strong> " . $row['state'] . "</p>";
                    echo "<p><strong>Country:</strong> " . $row['country'] . "</p>";
                    echo "</div>";
                }
            } else {
                echo "0 results";
            }
            $conn->close();
            ?>
        </div>
    </div>
    <script src="script.js"></script>
</body>
</html>
