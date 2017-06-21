<!DOCTYPE html>
<head>
    <title>Add Person</title>
</head>
<body>
    <?php
        include('header.php');
    ?>
    <h1>Add Person</h1>
    <form>
        Type of person are you adding: 
        <input type="radio" name="persontype" value="Actor/Actress" checked>Actor/Actress
        <input type="radio" name="persontype" value="Director">Director<br>
        First Name: <input type="text" name="first" maxlength=20 ><br>
        Last Name: <input type="text" name="last" maxlength=20><br>
        Sex:
        <input type="radio" name="sex" value="Male" checked>Male
        <input type="radio" name="sex" value="Female">Female <br> 
        DOB (YYYY-M-D): <input type="text" name="dob"><br>
        DOD (YYYY-M-D): <input type="text" name="dod"><br><br>
        Note: Empty fields will be interpreted as NULL and insertion may fail because of that<br>
        <input type="submit" value="Add Person"><br> 
    </form>
    <?php
        //If any of the values are set
        if ( isset($_GET["first"]) || isset($_GET["last"]) || isset($_GET["dob"]) || isset($_GET["dod"]) )
        {

            //Connect to database
            $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
                or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);

            $persontype = $_GET["persontype"];

            if ( empty(($first = $_GET["first"])) )
                $first = NULL;
            else
            {
                if (preg_match("/[^A-Za-z\s\'-]/", $first))
                    die("The first name is invalid");
                $first = trim($first);
                $first = $mysqli->real_escape_string($first);
            }
            if ( empty(($last = $_GET["last"])) )
                $last = NULL;
            else
            {
                if (preg_match("/[^A-Za-z\s\'-]/" , $last))
                   die("The last name is invalid"); 
                $last = trim($last);
                $last = $mysqli->real_escape_string($last);
            }

            $sex = $_GET["sex"];

            if ( empty(($dob = $_GET["dob"])) )
                $dob = NULL;

            if ( empty(($dod = $_GET["dod"])) )
                $dod = NULL;

            //Error checking date
            if ($dob != NULL)
            {
                $dob_array = date_parse($dob);
                if (!checkdate($dob_array["month"], $dob_array["day"], $dob_array["year"]))
                    die("Invalid birth date supplied");
            }

            if ($dod != NULL)
            {
                $dod_array = date_parse($dod);
                if (!checkdate($dod_array["month"], $dod_array["day"], $dod_array["year"]))
                    die("Invalid death date supplied");
            }


            //Get the current maximum id to assign the next one to the new person
            $result = $mysqli->query("SELECT Max(id) FROM MaxPersonID") 
                or die("Could not get the maximum ID number: " . $mysqli->error . $mysqli->errno);
            $row = $result->fetch_assoc();
            $oldID = $row['Max(id)'];
            $newID = $oldID+1;
            $result->free();

            //Form the query based on the person's type

            if ($persontype == "Actor/Actress")
                $query = "INSERT INTO Actor VALUES ('$newID', '$last', '$first', '$sex', '$dob', '$dod')";
            else //The person is a Director 
                $query = "INSERT INTO Director VALUES ('$newID', '$last', '$first', '$dob', '$dod')";

            $mysqli->autocommit(FALSE);

            if (!$mysqli->query($query))
            {
                $mysqli->rollback();
                die("Could not insert Actor/Actress or Director: " . $mysqli->error . $mysqli->errno);
            }


            if (!$mysqli->query("UPDATE MaxPersonID SET id='$newID' WHERE id='$oldID'"))
            {
                $mysqli->rollback();
                die("The MaxPersonID table could not be updated: " . $mysqli->error . $mysqli->errno);
            } 

            $mysqli->commit();
            print("Success! Able to add $first $last with ID: $newID to the database.");
            $mysqli->close();
        }

    ?>
</body>
</html>
