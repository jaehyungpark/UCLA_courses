<!DOCTYPE html>
<html>
<head>
        <title>Show Actor/Actress</title>
</head>

<?php
    include "header.php";
?>
<body>
<h1>Actor/Actress Information</h1>

<?php

    if (isset($_GET["aid"]))
    {
        $aid = $_GET["aid"];
        $query = "SELECT * FROM Actor WHERE (Actor.id = $aid)"; 
        $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
            or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);
        $result = $mysqli->query($query)
            or die("Could not find the Actor/Actress: " . $mysqli->error . $mysqli->errno);
        while ($row = $result->fetch_assoc())
        {
            $id = $row["id"];
            $last = $row["last"];
            $first = $row["first"];
            $sex = $row["sex"];
            $dob = $row["dob"];
            $dod = $row["dod"];
            print("Name: $first $last<br>");
            print("ID: $id<br>");
            print("Sex: $sex<br>");
            print("Date of Birth: $dob<br>");
            if ($dod == "0000-00-00")
                print("Date of Death: Still Alive");
            else
                print("Date of Death: $dod<br>");

        }
        $result->free();
        print("<h3>List of movies the actor has played in:</h3>");

        $query = "SELECT title, year, mid, role ".
                 "From Actor ".
                 "JOIN MovieActor ON Actor.id = MovieActor.aid ".
                 "JOIN Movie ON MovieActor.mid = Movie.id ".
                 "WHERE Actor.id = $id"; 

        $result = $mysqli->query($query)
            or die("Could not query the Actor/Actress's movies: " . $mysqli->error . $mysqli->errno);

        while ($row = $result->fetch_assoc())
        {
            $role = $row["role"];
            $title = $row["title"];
            $year = $row["year"];
            $mid = $row["mid"];
            print("Played \"$role\" in <a href=\"showMovie.php?mid=$mid\">$title ($year)</a><br>");
        }
        $result->free();
        $mysqli->close();
    }
?>

</body>
</html>
