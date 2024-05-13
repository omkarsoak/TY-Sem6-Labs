document.addEventListener('DOMContentLoaded', function() {
    const quizForm = document.getElementById('quiz-form');
    const questionsContainer = document.getElementById('questions-container');
    const scoreContainer = document.getElementById('score-container');
    const scoreDiv = document.getElementById('score');
    const resetButton = document.getElementById('reset-button');
    function fetchQuestions() {
        fetch('get_questions.php')
            .then(response => response.json())
            .then(data => {
                data.forEach((question, index) => {
                    const questionDiv = document.createElement('div');
                    questionDiv.className = 'question card';
                    questionDiv.innerHTML = `
                        <p>${index + 1}. ${question.question}</p>
                        <div class="options" id="options-${index}">
                            ${question.options.map((option, i) => `
                                <input type="radio" name="answer-${index}" value="${option}" id="option-${index}-${i}" data-correct-answer="${question.answer}">
                                <label for="option-${index}-${i}">${option}</label>
                                <br>
                            `).join('')}
                        </div>
                        <p class="correct-answer" style="display:none">Correct Answer: ${question.answer}</p>
                    `;
                    questionsContainer.appendChild(questionDiv);
                });
            })
            .catch(error => console.error('Error fetching questions:', error));
    }

    function calculateScore() {
        let totalQuestions = 0;
        let score = 0;
        let allQuestionsAnswered = true;
        document.querySelectorAll('.question').forEach((questionDiv, index) => {
            totalQuestions++;
            const selectedOption = questionDiv.querySelector(`input[name="answer-${index}"]:checked`);
            if (selectedOption) {
                if (selectedOption.value === selectedOption.getAttribute('data-correct-answer')) {
                    score++;
                }
            } else {
                allQuestionsAnswered = false;
            }
        });

        if (!allQuestionsAnswered) {
            alert('Please answer all questions before submitting.');
            return;
        }

        document.querySelectorAll('.question').forEach((questionDiv, index) => {
            const selectedOption = questionDiv.querySelector(`input[name="answer-${index}"]:checked`);
            if (selectedOption) {
                const label = questionDiv.querySelector(`label[for="${selectedOption.id}"]`);
                if (selectedOption.value === selectedOption.getAttribute('data-correct-answer')) {
                   label.classList.add('selected-answer-correct');
                } else {
                    label.classList.add('selected-answer-incorrect');
                }
            }
        });

        document.querySelectorAll('.correct-answer').forEach(correctAnswerPara => {
            correctAnswerPara.style.display = 'block';
        });

        scoreDiv.innerHTML = `<h4>${score} out of ${totalQuestions}</h4>`;
        scoreContainer.style.display = 'block';
    }

    quizForm.addEventListener('submit', function(event) {
        event.preventDefault();
        calculateScore();
    });

    resetButton.addEventListener('click', function() {
        questionsContainer.innerHTML = ''; 
        scoreContainer.style.display = 'none';
        fetchQuestions();
    });

    fetchQuestions();
});

