-- Query for actors in Die Another Day
SELECT CONCAT(first, ' ', last)
FROM Actor
JOIN MovieActor
JOIN Movie
WHERE Actor.id = MovieActor.aid 
AND MovieActor.mid = Movie.id 
AND Movie.title = 'Die Another Day';

-- Count of actor who have appeared in multiple movies
SELECT COUNT(DISTINCT ma1.aid)
FROM MovieActor ma1, MovieActor ma2
WHERE ma1.mid<>ma2.mid 
AND ma1.aid=ma2.aid;

-- Title of movies that sell more than 1,000,000 tickets
SELECT title
FROM Movie, Sales
WHERE Movie.id = Sales.mid
AND ticketsSold > 1000000;

-- Count of actors who have played in an Adventure movie
SELECT COUNT(DISTINCT first, last)
FROM Actor, MovieActor, Movie, MovieGenre
WHERE Actor.id = MovieActor.aid
AND MovieActor.mid = Movie.id
AND Movie.id = MovieGenre.mid
AND MovieGenre.genre = 'Adventure';

-- Find the maximum number of tickets sold for an Action movie
SELECT MAX(Sales.ticketsSold)
FROM Movie, MovieGenre, Sales
WHERE Movie.id = MovieGenre.mid
AND MovieGenre.genre = 'Action'
AND Movie.id = Sales.mid;
