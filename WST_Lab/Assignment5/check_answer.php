<?php
$answer = json_decode(file_get_contents('php://input'), true)['answer'];
$correctAnswer = '';

if ($answer == $correctAnswer) {
    $response = array('message' => 'Correct!');
} else {
    $response = array('message' => 'Incorrect. Please try again.');
}

echo json_encode($response);
?>