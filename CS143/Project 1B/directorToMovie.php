<!DOCTYPE html>
<html>
<head>
    <title>Add Director to Movie</title>
</head>
<body>
    <?php
        include('header.php');
    ?>
    <h1> Add Director to Movie</h1>
    Select the name of a director that you want to associate a movie with.<br><br>
    <form action=# method="GET">

<?php
        $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
            or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);

        $result = $mysqli->query("SELECT * FROM Movie ORDER BY title ASC")
            or die("Could not query Movie: " . $mysqli->error . $mysqli->errno);
        $movieOptions="";
        while ($row=$result->fetch_assoc())
        {
            $id=$row["id"];
            $title=$row["title"];
            $year=$row["year"];
            $movieOptions.="<option value=\"$id\">".$title." (".$year.")</option>";
        }
        $result->free();

        $result = $mysqli->query("SELECT * FROM Director ORDER BY first ASC")
            or die("Could not query Director: " . $mysqli->error . $mysqli->errno);
        $directorOptions = "";
        while ($row = $result->fetch_assoc())
        {
            $id=$row["id"];
            $first=$row["first"];
            $last=$row["last"];
            $directorOptions.="<option value=\"$id\">".$first." ".$last."</option>";
        }
        $result->free();
?>
            Movie:  <select name="mid">
                        <?=$movieOptions?>
                    </select><br>
            Director:  <select name="did">
                        <?=$directorOptions?>
                    </select><br><br>
            <input name= "submitbutton" type="submit" value="Submit">
        </form>

        <?php
            if (isset($_GET["submitbutton"]))
            {
                $movieID = $_GET["mid"];
                $directorID = $_GET["did"];
                $result = $mysqli->query("INSERT INTO MovieDirector VALUES('$movieID', '$directorID')")
                    or die ("Failed to add movie and director association: " . $mysqli->error . $mysqli->errno);
                print("Director successfully linked with Movie: " . (string)$directorID . " >> " . (string)$movieID);
                $result->free();
            }
            $mysqli->close();
        ?>
</body>
</html>
