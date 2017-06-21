<!DOCTYPE html>
<html>
<head>
        <title>Show Movie</title>
</head>
<?php
    include('header.php');
?>

<body>
<h1>Movie Information</h1>

<?php
    $mid = $_GET["mid"];
    $query = "SELECT * FROM Movie WHERE (Movie.id = $mid)";
    $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
        or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);
    $result = $mysqli->query($query)
        or die("Could not query Movie: " . $mysqli->error . $mysqli->errno);
    while ($row = $result->fetch_assoc()) 
    {
        $mid = $row["id"];
        $title = $row["title"];
        $year = $row["year"];
        $rating = $row["rating"];
        $company = $row["company"];
        print("Title: $title<br>");
        print("ID: $mid<br>");
        print("Year: $year<br>");
        print("Rating: $rating<br>");
        print("Company: $company<br>");

        $secondquery = "SELECT d.last, d.first FROM MovieDirector md, Director d WHERE md.mid=$mid AND md.did=d.id";
        $result1 = $mysqli->query($secondquery) 
            or die("Could not query Director: " . $mysqli->error . $mysqli->errno);
        $row_count = $result1->num_rows;
        print("Director(s):  ");
        if ($row_count == 0)
            echo "<br>";
        while($row1 = $result1->fetch_assoc())
        {
            $row_count--;
            $first = $row1["first"];
            $last = $row1["last"];
            print("$first $last");
            if ($row_count == 0)
                print("<br>");
            else
                print(", ");
        }
        $result1->free();
        

        $thirdquery = "SELECT genre from MovieGenre WHERE mid=$mid";
        $result2 = $mysqli->query($thirdquery)
            or die ("MovieGenre could not be queried: " . $mysqli->error . $mysqli->errno);
        $row_count = $result2->num_rows;
        print("Genre: ");
        if ($row_count == 0)
            print("<br>");
        while ($row2 = $result2->fetch_assoc())
        {
            $row_count--;
            $genre = $row2["genre"];
            print("$genre");
            if ($row_count == 0)
                print("<br>");
            else 
                print(", ");
        }
        $result2->free();
    }
    $result->free();

    print("<h3>List of actors in the movie:</h3>");

    $query = "SELECT first, last, id, role ".
             "FROM Actor ".
             "JOIN MovieActor ON (MovieActor.mid = $mid) AND  (Actor.id = MovieActor.aid)";
        
    $result = $mysqli->query($query)
        or die ("Actor could not be queried: " . $mysqli->error . $mysqli->errno);
    while ($row = $result->fetch_assoc())
    {
        $role = $row["role"];
        $first = $row["first"];
        $last = $row["last"];
        $id = $row["id"];
        print("<a href=\"showActor.php?aid=$id\">$first $last</a> as \"$role\"<br>");
    }
    $result->free();
    
    print("<h3>User Reviews</h3>");
    $query = "SELECT AVG(rating), COUNT(rating) FROM Review WHERE mid=$mid";
    $result = $mysqli->query($query) 
        or die("Cannot query from Review: " . $mysqli->error . $mysqli->errno);
    $row = $result->fetch_assoc();
    $avg = $row["AVG(rating)"];
    $count = $row["COUNT(rating)"];
    if ($avg == "")
        print("There has been no reviews (thus no average) for this movie.");
    else
        print("Average Rating: $avg out of 5 for $count people");
    print(str_repeat('&nbsp;', 4));
    print("<a href=\"movieReview.php?mid=$mid\">Add your own review</a><br><br>");  
    $result->free();

    $query = "SELECT * FROM Review WHERE mid=$mid ORDER BY time DESC";
    $result = $mysqli->query($query) 
        or die("Cannot query from Review: " . $mysqli->error . $mysqli->errno);
    while ($row = $result->fetch_assoc())
    {
        $time = $row["time"];
        $rating = $row["rating"];
        $name = $row["name"];
        $comment = $row["comment"];
        print("Time: $time<br>");
        print("Name: $name<br>");
        print("Rating: $rating<br>");
        print("Comment: $comment<br><br>");
    }
    $result->free();
    $mysqli->close();
?>

</body>
</html>
