<!DOCTYPE html>
<html>
<head>
    <title>Add Movie</title>
</head>
<body>
    <?php
        include('header.php');
    ?>
    <h1> Add Movie</h1>
    In this page you can add a movie and its pertinent information.
    <form>
        Title: <input type="text" name="title" maxlength="100"><br>
        Year: <input type="text" name="year"><br>
        MPAA Rating: <input type="text" name="rating" maxlength="10"><br>
        Company: <input type="text" name="company" maxlength="50"><br>
        Genre: <input type="text" name="genre"><br><br>
        Enter genre as a list separated by a comma and space (e.g.: Hello, goodbye, etc...)<br>
        <input type="submit" value="Submit">
    </form>

    <?php
        if (isset($_GET["title"])) //If a title is submitted start the process
        {
            //Connect to database
            $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
                or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);

            //Do stuff with title 
            $title = $_GET["title"];
            if (empty($title))
                $title = NULL;
            else
                $title = trim($title);
            if(preg_match('/[^A-Za-z0-9\s\'-,:.]/', $title))
                die("Your title's name is invalid");
            $title = $mysqli->real_escape_string($title);

            //Do stuff with year
            $year = $_GET["year"];
            if (empty($year))
                $year = NULL;
            else
                $year = trim($year);
            if ($year != NULL && !preg_match('/^[0-9]{0,4}$/', $year))    //Must be <= 4 digits
                die("The movie's year is invalid");

            //Do stuff with rating
            $rating = $_GET["rating"];
            if (empty($rating))
                $rating = NULL;
            else
                $rating = trim($rating);
            if (preg_match('/[^A-Za-z0-9\s\'-.]/', $rating))
                die("The movie's rating is invalid");
            $rating = $mysqli->real_escape_string($rating);

            //Do stuff with company
            $company = $_GET["company"];
            if (empty($company))
                $company = NULL;
            else
                $company = trim($company);
            if (preg_match('/[^A-Za-z0-9\s\'-,.]/', $company))
                die("The movie's company is invalid.");
            $comapny = $mysqli->real_escape_string($company);

            $genre = $_GET["genre"];
            $genre = trim($genre);
            if (empty($genre))
                $genre = NULL;
            else
            {
                $genre = explode(", ", $genre);
                for ($i=0; $i<count($genre); $i++)
                {
                    if (preg_match('/[^A-Za-z0-9\s\'-,]/', $genre[$i]) || strlen($genre[$i]) > 20 )
                        die("One of the genre in the list is invalid.");
                    $genre[$i] = $mysqli->real_escape_string($genre[$i]);
                }
            }


            $result = $mysqli->query("SELECT MAX(id) FROM MaxMovieID")
                or die("Could not query MaxMovieID: " . $mysqli->error . $mysqli->errno);

            $row = $result->fetch_assoc();
            $oldID = $row['MAX(id)'];
            $newID = $oldID + 1;
            $result->free();

            $query = "INSERT INTO Movie VALUES ('$newID', '$title', '$year', '$rating', '$company')";

            $mysqli->autocommit(FALSE);

            if (!$mysqli->query($query))
            {
                $mysqli->rollback();
                die("Could not add Movie " . $mysqli->error . $mysqli->errno);
            }

            if (!$mysqli->query("UPDATE MaxMovieID SET id='$newID' WHERE id='$oldID'"))
            {
                $mysqli->rollback();
                die("The MaxMovieID table could not be updated: " . $mysqli->error . $mysqli->errno);
            }

            if($genre[0] != NULL)
                for ($i=0; $i < count($genre); $i++)
                    if (!$mysqli->query("INSERT INTO MovieGenre VALUES ('$newID','$genre[$i]')"))
                    {
                        $mysqli->rollback();
                        die("Could not query in the MovieGenre table: " . $mysqli->error . $mysqli->errno);
                    }

            $mysqli->commit();

            print("Successfully Added $title with ID: $newID to the database.");
            $mysqli->close();
        }
    ?>
</body>
</html>
