<!DOCTYPE html>
<html>
<head>
    <title>Review a Movie</title>
</head>
<body>
    <?php
        include('header.php');
        $default_mid = $_GET["mid"]; 
    ?>
    <h1> Write a Movie Review </h1>
    Please enter a review for the movie in the textbox below.
    <br>
    <form>
    Name: <input type="text" name="name" maxlength="20"><br>
    Movie ID: <input type="text" name="mid" value="<?php print($default_mid); ?>"><br>
    Rating out of 5: <input type="text" name="rating" maxlength="1"><br>
    Comment:<br>
    <textarea name="comment" style="min-height:100px;height:100%;width:100%" maxlength="500"></textarea><br>
    <input type="submit" name="submit">
    </form>
    <?php
        if (isset($_GET["mid"]) && isset($_GET["submit"]) && isset($_GET["rating"]))
        {
            $mysqli = new mysqli('localhost', 'cs143', '', 'CS143')
                or die("Could not connect to database: " . $mysqli->error . $mysqli->connect_errno);

            $mid = $_GET["mid"]; 

            $name = $_GET["name"];
            if (empty($name))
                $name = NULL;
            else 
                $name = trim($name);

            if(preg_match('/[^A-Za-z\s\'-,]/', $name))
                die("Your name is invalid (Probably Special Characters)");
            $name = $mysqli->real_escape_string($name);

            $rating = $_GET["rating"];
            if ($rating < "0" || $rating > "5")
                die("Your rating is an invalid number");

            $comment = $_GET["comment"];
            if (empty($comment))
                $comment = NULL;
            else
                $comment = $mysqli->real_escape_string($comment);
            $query = "INSERT INTO Review VALUES ('$name', '$query_timestamp', '$mid', '$rating', '$comment')";

            $mysqli->autocommit(FALSE);

            if (!$result = $mysqli->query($query))
            {
                $mysqli->rollback();
                die("Could not insert into Review Table: " . $mysqli->error .  $mysqli->errno);
            }
            if (!$mysqli->query("UPDATE Review SET time=now() WHERE (name='$name' AND mid='$mid'AND rating='$rating' AND comment='$comment')"))
            {
                $mysqli->rollback();
                die("Could not update your timestamp: " . $mysqli->error . $mysqli->errno);
            }
            $mysqli->commit();
            print("You successfully added your review! Click <a href=\"showMovie.php?mid=$mid\">here</a> to go back to the movie." );
            $result->free();
            $mysqli->close();
        }
    ?>
</body>
</html>

