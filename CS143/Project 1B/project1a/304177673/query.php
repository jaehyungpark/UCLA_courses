<!DOCTYPE html>
<html>
<head>
	<title>CS143 Project 1A</title>
</head>
<body>
<h1>CS 143 Project 1A</h1>
This website allows you to query the MySQL database from the Project 1A
specification.<br>
Enter your query below in the text box and press Submit.
<p>
<form action="#" method="GET">
<textarea name="query" rows="10" cols="60">
</textarea><br>
<input type="submit" value="query">
</form>
</p>
</body>
</html>

<?php
$query =$_GET["query"];
if (!empty($query))
{
	$db_connection = mysql_connect('localhost', 'cs143', '');
	if (!$db_connection)
		die('Could not connect: ' . mysql_error()); 
	$db_selected = mysql_select_db('CS143', $db_connection); //change this LATER to CS143
	if (!$db_selected)
		die ('Could not select the CS143 Database. ' . mysql_error());
	$result = mysql_query($query, $db_connection);
//Not sanatizing output because spec allows us to assume all SELECT
//queries are valid
	if (!$result)
		die('Not a valid query.');
	$fields = mysql_num_fields($result);
	print"<table border=0>";
	for ($element = 0; $element < $fields; $element++)
	{
		$fieldsvar = mysql_fetch_field($result, $element);
		print "<th>";
		print "$fieldsvar->name";
		print "</th>";
	}
	print"\n";
	while ($row = mysql_fetch_row($result))
	{
		print "<tr>";
		foreach ($row AS $cell)
			print "<td>$cell</td>";
		print "</tr>\n";
	}
	mysql_free_result($result);
	mysql_close($db_connection);
}
?>
