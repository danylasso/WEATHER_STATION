-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 06-03-2022 a las 22:28:21
-- Versión del servidor: 10.4.14-MariaDB
-- Versión de PHP: 7.2.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `temphumidnew`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `dht11`
--

CREATE TABLE `dht11` (
  `ID` int(11) NOT NULL,
  `humidity` float(4,2) NOT NULL,
  `temperature` float(4,2) NOT NULL,
  `pressure` float(6,2) NOT NULL,
  `altitude` float(6,2) NOT NULL,
  `lluvia` int(1) NOT NULL,
  `date` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Volcado de datos para la tabla `dht11`
--

INSERT INTO `dht11` (`ID`, `humidity`, `temperature`, `pressure`, `altitude`, `lluvia`, `date`) VALUES
(1, 93.00, 22.45, 826.39, 1686.76, 1, '2022-03-06 21:21:25'),
(2, 93.00, 22.47, 826.38, 1686.82, 1, '2022-03-06 21:21:36'),
(3, 93.00, 22.48, 826.39, 1686.81, 1, '2022-03-06 21:21:47'),
(4, 93.00, 22.51, 826.38, 1686.82, 1, '2022-03-06 21:21:57'),
(5, 94.00, 22.53, 826.40, 1686.65, 1, '2022-03-06 21:22:08');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `dht11`
--
ALTER TABLE `dht11`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `dht11`
--
ALTER TABLE `dht11`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
