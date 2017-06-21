-- Every Movie has a unique id
-- Movie Ratings must be either null, G, PG, PG-13, R, or NC-17

CREATE TABLE Movie(
	id int NOT NULL,
	title varchar(100),
	year int NOT NULL,
	rating varchar(10),
	company varchar(50),
	PRIMARY KEY (id),
	CHECK(rating IS NULL OR rating='G' OR rating='PG' OR rating='PG-13' OR rating='R' OR rating='NC-17')
) ENGINE=INNODB;

-- Every Actor has a unique id
CREATE TABLE Actor(
	id int NOT NULL,
	last varchar(20),
	first varchar(20),
	sex varchar(6),
	dob date,
	dod date,
	PRIMARY KEY (id)
) ENGINE=INNODB;

-- Sale's mid refers to a movie's id
CREATE TABLE Sales(
    mid int NOT NULL,
    ticketsSold int NOT NULL,
    totalIncome int NOT NULL,
    FOREIGN KEY (mid) references Movie(id)
) ENGINE=INNODB;

-- Every director has a unique id
CREATE TABLE Director(
	id int NOT NULL,
	last varchar(20),
	first varchar(20),
	dob date,
	dod date,
	PRIMARY KEY (id)
) ENGINE=INNODB;

-- MovieGenre's mid refers to a movie's id
CREATE TABLE MovieGenre(
	mid int NOT NULL,
	genre varchar(20) NOT NULL,
	FOREIGN KEY (mid) references Movie(id)
) ENGINE=INNODB;

-- A MovieDirector's mid refers to a movie's id
-- A MovieDirector's did refers to a director's id
CREATE TABLE MovieDirector(
    mid int,
    did int,
    FOREIGN KEY (mid) references Movie(id),
    FOREIGN Key (did) references Director(id)
) ENGINE=INNOOB;

-- A MovieActor's mid refers to a movie's id
-- A MovieActor's aid refers to an actor's id
CREATE TABLE MovieActor(
	mid int NOT NULL,
	aid int NOT NULL,
	role varchar(50),
	FOREIGN KEY (mid) references Movie(id),
	FOREIGN KEY (aid) references Actor(id)
) ENGINE=INNODB;

-- A MovieRating's mid refers to a movie's id
-- A MovieRating's rot must be between 0 and 100
-- A MovieRating's imdb must be between 0 and 100
CREATE TABLE MovieRating(
    mid int NOT NULL,
    imdb int NOT NULL,
    rot int NOT NULL,
    CHECK(rot >= 0 AND rot <= 100),
    CHECK(imdb >= 0 AND imdb <=100),
    FOREIGN KEY (mid) references Movie(id)
) ENGINE=INNODB;

-- A Review's mid refers to a movie's id
-- Review ratings must be between 0 and 5
CREATE TABLE Review(
	name varchar(20),
	time timestamp,
	mid int NOT NULL,
	rating int NOT NULL,
	comment varchar(500),
	FOREIGN KEY (mid) references Movie(id),
	CHECK(rating >= 0 AND rating <= 5)
) ENGINE=INNODB;

CREATE TABLE MaxPersonID(
	id int NOT NULL
) ENGINE=INNODB;

CREATE TABLE MaxMovieID(
	id int NOT NULL
) ENGINE=INNODB;
