<!DOCTYPE html>
<head>
    <title>Search</title>
</head>
<body>
    <?php
        include('header.php');
    ?>
    <h1>Search for Actor/Actress or Movie</h1>
    <p>Enter the the name of the actor, actress, or movie to look up.</p>
    <form>
        Enter search terms:
        <input type="text" name="search">
        <input name= "submitbutton" type="submit" value="submit"></form>
        <br><br>
    </form>
        <?php
            if (isset($_GET["submitbutton"]))
            {
                if (empty($_GET["search"]))
                    die("A search term was not entered.");

                $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
                    or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);
                $search = $_GET["search"];
                $search = $mysqli->real_escape_string($search);
                $search_array = explode(" ", strtolower($search));
                $search_type = $_GET["searchtype"];
                $size_of_search = count($search_array);
                $query = "SELECT first, last, id, dob " .
                         "FROM Actor " .
                         "WHERE ";
                for ($i = 0; $i < $size_of_search; $i++)
                { 
                    $query = $query . "(LOWER(first) like '%$search_array[$i]%' OR LOWER(last) LIKE '%$search_array[$i]%')";
                    if ($i != $size_of_search-1)
                        $query = $query. " AND ";
                }
                $result = $mysqli->query($query)
                    or die ("Failed to search Actor: " . $mysqli->error . $mysqli->errno);
                print("<h2>Matching actors/actresses</h2>");
                while ($row = $result->fetch_assoc())
                {
                    $id = $row["id"];
                    $first = $row["first"];
                    $last = $row["last"];
                    $dob = $row["dob"];
                    print("<a href=\"showActor.php?aid=$id\">$first $last ($dob)</a><br>"); 
                }
                $result->free();
                $query = "SELECT title, year, id " .
                         "FROM Movie " .
                         "WHERE ";
                for ($i = 0; $i < $size_of_search; $i++)
                {
                    $query = $query . "(LOWER(title) like '%$search_array[$i]%')";
                    if ($i != $size_of_search-1)
                        $query = $query . " AND ";
                }
                $result = $mysqli->query($query)
                    or die ("Failed to search Movie: " . $mysqli->error . $mysqli->errno);
                print("<h2>Matching movies</h2>");
                while ($row = $result->fetch_assoc())
                {
                    $title = $row["title"];
                    $year = $row["year"];
                    $id = $row["id"];
                    print("<a href=\"showMovie.php?mid=$id\">$title ($year)</a><br>");
                }
                $result->free();
                $mysqli->close();
            }
        ?>
</body>
</html>
