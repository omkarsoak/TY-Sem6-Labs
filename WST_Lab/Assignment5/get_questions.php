<?php
$questions = array(
    array(
        'question' => 'What does the &lt;canvas&gt; element in HTML5 allow you to do?',
        'options' => array('Embed 3D models directly into a webpage', 'Create animated GIFs', 'Draw graphics, animations, or images dynamically using JavaScript', 'Embed audio files directly into a webpage'),
        'answer' => 'Draw graphics, animations, or images dynamically using JavaScript'
    ),
    array(
        'question' => 'Which property is used to change the color of text in CSS?"?',
        'options' => array('background-color', 'color', 'text-color', 'font-color'),
        'answer' => 'color'
    ),
    array(
        'question' => 'What is the purpose of the addEventListener method in JavaScript?',
        'options' => array('To change the CSS properties of an element', 'To create a new HTML element', 'To attach an event handler to an element', 'To execute a function immediately upon page load'),
        'answer' => 'To attach an event handler to an element'
    ),
    array(
        'question' => 'What does PHP stand for?',
        'options' => array('Personal Home Page', 'Preprocessed Hypertext Processor', 'Professional Hypertext Programming', 'Public Hosted Platform'),
        'answer' => 'Personal Home Page'
    ),
    array(
        'question' => 'Which of the following protocols is commonly used for communication between web services?',
        'options' => array('HTTP (Hypertext Transfer Protocol)', 'FTP (File Transfer Protocol)', 'SMTP (Simple Mail Transfer Protocol)', 'SNMP (Simple Network Management Protocol)'),
        'answer' => 'HTTP (Hypertext Transfer Protocol)'
    )
);

echo json_encode($questions);
?>
