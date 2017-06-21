<!DOCTYPE html>
<html>
<head>
    <title>Add Actor/Actress to Movie</title>
</head>
<body>
    <?php
        include('header.php');
    ?>
    <h1>Add Actor/Actress to Movie</h1>
    Select the name of an actor/actress that you want to associate a movie with.<br><br>
    <form action=# method="GET">

<?php
        $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
            or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);

        $result = $mysqli->query("SELECT * FROM Movie ORDER BY title ASC") 
            or die ("Could not query Movie: " . $mysqli->error . $mysqli->errno);
        $movieOptions="";
        while ($row = $result->fetch_assoc())
        {
            $id = $row["id"];
            $title = $row["title"];
            $year = $row["year"];
            $movieOptions.="<option value=\"$id\">".$title." (".$year.")</option>";
        }
        $result->free();

        $result = $mysqli->query("SELECT * FROM Actor ORDER BY first ASC")
            or die ("Could not query Movie: " . $mysqli->error . $mysqli->errno);
        $actorOptions="";
        while ($row=$result->fetch_assoc())
        {
            $id=$row["id"];
            $first=$row["first"];
            $last=$row["last"];
            $actorOptions.="<option value=\"$id\">".$first." ".$last."</option>";
        }
        $result->free();
?>

            Movie:  <select name="mid">
                        <?=$movieOptions?>
                    </select><br>
            Actor:  <select name="aid">
                        <?=$actorOptions?>
                    </select><br>

            Role: <input type="text" name="role" value="<?php echo htmlspecialchars($_GET['role']);?>" maxlength="50"><br/>
            <br>
            <input name="submitbutton" type="submit" value="Submit">
        </form>

        <?php
            if (isset($_GET["submitbutton"]))
            {
                $movieID = $_GET["mid"];
                $actorID = $_GET["aid"];
                $role = $_GET["role"];
                $role = $mysqli->real_escape_string($role);

                $result = $mysqli->query("INSERT INTO MovieActor VALUES('$movieID', '$actorID', '$role')")
                    or die ("Failed to insert movie and actor/actress association: " . $mysqli->error . $mysqli->errno);
                print("Actor/Actress successfully linked with Movie: " . (string)$actorID ." >> " . (string)$movieID . " " . $role);
                $result->free();
            }
            $mysqli->close();
        ?>
</body>
</html>
