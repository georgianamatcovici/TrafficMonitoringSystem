CREATE TABLE Cars (
    Username TEXT NOT NULL UNIQUE,
    Password TEXT NOT NULL
);

INSERT INTO Cars (Username, Password) VALUES 
('car1', 'psswdcar1'),
('car2', 'psswdcar2'),
('car3', 'psswdcar3'),
('car4', 'psswdcar4'),
('car5', 'psswdcar5'),
('car6', 'psswdcar6'),
('car7', 'psswdcar7'),
('car8', 'psswdcar8'),
('car9', 'psswdcar9'),
('car10', 'psswdcar10');

CREATE TABLE Streets (
    Id INT,
    Name TEXT NOT NULL,
    Speed INT,
    SpeedLimits INT,
    Length REAL,
    Accident INT,
    GasStation TEXT,
    Weather TEXT
);

INSERT INTO Streets (Id, Name, Speed, SpeedLimits, Length, Accident, GasStation, Weather) 
VALUES 
(1, 'Pacurari', 50, 1, 7.3, 0, 'OMV', 'Sunny'),
(2, 'Nationala', 40, 2, 10.9, 0, 'Rompetrol', 'Rainy'),
(3, 'Independentei', 50, 1, 12.1, 2, 'Lukoil', 'Cloudy'),
(4, 'Garii', 40, 3, 4.3, 0, 'Petrom', 'Rainy'),
(5, 'Industriala', 30, 4, 1.7, 2, 'No;Libertatii;MOL', 'Rainy'),
(6, 'Libertatii', 40, 5, 3.7, 0, 'MOL', 'Rainy'),
(7, 'Lalelelor', 30, 6, 1.4, 0, 'No;Nationala;Rompetrol', 'Rainy'),
(8, 'Arcu', 30, 4, 0.5, 0, 'No;Pacurari;OMV', 'Rainy'),
(9, 'Florilor', 40, 2, 1.4, 2, 'No;Pacurari;OMV', 'Rainy'),
(10, 'Palat', 50, 8, 2.5, 0, 'Lukoil', 'Rainy'),
(11, 'V. Alecsandri', 40, 3, 0.5, 0, 'No;Nationala;Rompetrol', 'Rainy'),
(12, 'Cuza Voda', 30, 6, 2.2, 0, 'MOL', 'Rainy'),
(13, 'Aurel Vlaicu', 50, 7, 5.5, 0, 'Petrom', 'Rainy'),
(14, 'Unirii', 30, 5, 1, 0, 'No;Pacurari;OMV', 'Rainy'),
(15, 'Nicolina', 40, 2, 5.2, 2, 'MOL', 'Rainy'),
(16, 'Ion Creanga', 50, 7, 3.8, 0, 'No;Independentei;Lukoil', 'Rainy'),
(17, 'Dimitrie Cantemir', 30, 4, 3.8, 2, 'No;Pantelimon;OMV', 'Rainy'),
(18, 'Morilor', 40, 3, 4.6, 0, 'No;Parcului;MOL', 'Rainy'),
(19, 'Parcului', 30, 2, 5.2, 0, 'MOL', 'Rainy'),
(20, 'Pantelimon', 40, 5, 3.4, 0, 'OMV', 'Rainy'),
(21, 'Anastasie Panu', 50, 1, 5, 2, 'No;Independentei;Lukoil', 'Rainy'),
(22, 'Izvoarelor', 50, 7, 3.7, 0, 'No;Garii;Petrom', 'Rainy'),
(23, 'Lapusneanu', 50, 7, 5.1, 0, 'OMV', 'Rainy');

CREATE TABLE GasStations (
    Name TEXT NOT NULL,
    Type TEXT NOT NULL,
    Price REAL
  
);

INSERT INTO GasStations(Name, Type, Price)
VALUES
('Petrom', 'Standard Diesel', 7.280),
('Petrom', 'Standard Gasoline', 7.100),
('Petrom', 'Premium Gasoline 99', 7.660),
('Rompetrol', 'Gasoline 95', 7.15),
('Rompetrol', 'Premium Diesel', 7.73),
('Rompetrol', 'Standard Diesel', 7.32),
('OMV', 'MaxxMotion Diesel', 7.880),
('OMV', 'MaxxMotion 95', 7.140),
('OMV', 'MaxxMotion 100plus', 7.850),
('OMV', 'Diesel', 7.320),
('MOL', 'Gasoline 95', 7.45),
('MOL', 'Diesel', 8.15),
('MOL', 'Diesel Plus', 8.60),
('Lukoil', 'Diesel', 7.18),
('Lukoil', 'Super Diesel', 8.12),
('Lukoil', 'Gasoline 98', 7.65),
('Lukoil', 'Gasoline 95', 6.11);

CREATE TABLE SportEvents (
    Id INT,
    Text TEXT
  
);

INSERT INTO SportEvents(Id, Text)
VALUES
(1, 'Join us at 7:00 PM for an exciting basketball game between CSO Voluntari and SCM U Craiova at Sala Polivalenta!'),
(2, 'Handball action tonight! Steaua Bucuresti faces Potaissa Turda at Sala Polivalenta, 8:00 PM.'),
(3, 'Join us at 6:00 PM for an exciting handball game between Politehnica Timisoara and CSU Suceava at Sala Polivalenta!'),
(4, 'Join us tonight at 7:00 PM for an exciting volleyball match between CSM Bucuresti and VCM Zalau at Sala Polivalenta!'),
(5, 'Join us tonight at 8:00 PM for an exciting volleyball match between Volei Alba Blaj and CSM PLoiesti at Sala Polivalenta!'),
(6, 'Do not miss the action! MMA event tonight at 7:00 PM at Sala Polivalenta!'),
(7, 'It is fight night! Watch tonights kickboxing match at 9:00 PM, Sala Polivalenta!'),
(8, 'Do not miss tonight s gymnastics competition at 7:00 PM, featuring top athletes at Sala Polivalenta!'),
(9, 'It is gymnastics time! See amazing routines at 5:00 PM tonight—be there!'),
(10, 'Watch the best gymnasts in action! Gymnastics competition starts at 7:00 PM tonight, Sala Polivalenta. '),
(11, 'Latest basketball news: Gabriel Deck suffered an injury during the ACB League El Clssico against FC Barcelona.'),
(12, 'Latest handball news: Goalkeeper Anna Kristensen lit up the Women''s EHF EURO 2024 with her stunning performances.'),
(13, 'Latest handball news: Croatia & Germany stay top of beach handball national rankings.'),
(14, 'Latest judo news: Teddy Riner''s opponent was disqualified from the men''s 100kg+ judo tournament after reacting aggressively to Riner celebrating reaching the semi-finals.'),
(15, 'Latest kickboxing news: Ariel Machado Scores Three Big Knockouts to Claim Victory at the K-1 World Grand Prix 2024 in Tokyo.'),
(16, 'Latest news in gymnastics: Chile''s appeal to keep Olympic gymnastics medal fails.'),
(17, 'Latest news in gymnastics: People are obsessed with ‘pommel horse guy,’ aka USA gymnast Stephen Nedoroscik.'),
(18, 'Latest news in gymnastics: Bela Karolyi, gymnastics coach who mentored Nadia Comaneci, dies aged 82.'),
(19, 'Do not miss the football match tonight at 7:00 PM between FCSB and Dinamo Bucuresti at Stadionul National!'),
(20, 'Tonight at 8:00 PM, Politehnica Iasi faces Farul Constanta in a thrilling football match at Stadionul National!'),
(21, 'Get ready for an  an exciting football game! CFR Cluj vs Universitatea Craiova — tonight at 6:00 PM, Stadionul National.'),
(22, 'Latest football news: Premier League reaction: Liverpool go eight points clear as Forest move up to second.'),
(23, 'Latest football news: Pep Guardiola celebrated 500 games in charge of Manchester City with a nervy win over Leicester.'),
(24, 'Latest football news: Paul Pogba''s brother sentenced to three years in extortion case.'),
(25, 'Latest football news: Cristiano Ronaldo refuses to rule out stunning Man City move despite Man Utd past.'),
(26, 'Do not miss the Formula 1 race at 3:00 PM today at Pista de Formula 1! Get ready for thrilling action!'),
(27, 'It is race day! Watch the best drivers compete in the Formula 1 Grand Prix at 3:00 PM!'),
(28, 'Latest Formula 1 news: Leclerc singles out ''highlight of the season'' for Ferrari as he hails ''good job''.'),
(29, 'Latest Formula 1 news: Mercedes opens up on the factors that hurt Hamilton with the W15'),
(30, 'Do not miss the exciting figure skating competition tonight at 7:00 PM at Patinoarul Olimpic!'),
(31, 'The best figure skaters are competing tonight at Patinoarul Olimpic! Join us at 8:00 PM.'),
(32, 'Latest skating news: Kaori Sakamoto achieves historic ''three-peat'' at World Figure Skating Championships in Montreal.'),
(33, 'Latest skating news: Adam Siao Him Fa wows with stunning backflip to secure bronze in style at World Championships.'),
(34, 'Do not miss the exciting swimming competition at 7:00 PM tonight at Complexul Olimpic de Natatie!'),
(35, 'Watch the fastest swimmers compete live at 6:00 PM tonight! Join us at Complexul Olimpic de Natatie.'),
(36, 'Latest swimming news: Magnificent Maskill wins third gold in Paris.'),
(37, 'Latest swimming news: David Popovici “Open” To Facing Leon Marchand in 200 Freestyle.'),
(38, 'The best tennis players are competing tonight at the Tennis Club! Join us at 8:00 PM.'),
(39, 'Latest tennis news: Novak Djokovic targeting consistency and titles in 2025, calls Andy Murray partnership ''strange'' but ''terrific''.'),
(40, 'Latest tennis news: Rafael Nadal admits he almost took ''complete break'' from tennis - ''We are human beings, not superheroes''.');

CREATE TABLE Weather (
    Id INT,
    Text TEXT
  
);

INSERT INTO Weather(Id, Text)
VALUES
(1, 'Road surfaces may become slippery during rain. We recommend to reduce your speed.'),
(2, 'Temperatures will gradually rise, reaching up to 25°C'),
(3, 'A yellow weather warning for heavy rain and hail is in place until tommorrow.'),
(4, 'Winds will blow moderately from the northwest.'),
(5, 'A cold wave is forecast for next week.'),
(6, 'The weather will remain mostly sunny throughout the weekend, with temperatures around 22°C.'),
(7, 'There is a chance of light rain on Monday morning.'),
(8, 'Overnight temperatures will drop to around 10°C.'),
(9, 'There is dense fog this morning. Please drive carefully and keep your headlights on'),
(10, 'Heavy rain is expected later today.');
