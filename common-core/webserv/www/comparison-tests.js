// Comparison testing functions
function testGet() {
    const results = document.getElementById('basic-results');
    results.innerHTML = 'Testing GET requests...';
    
    Promise.all([
        fetch('/'),  // Your webserv
        fetch('http://localhost:8080/')  // nginx 1.1
    ]).then(responses => {
        Promise.all(responses.map(r => r.text())).then(bodies => {
            results.innerHTML = `
                <div>WebServ response length: ${bodies[0].length}</div>
                <div>nginx 1.1 response length: ${bodies[1].length}</div>
                <div>Match: ${bodies[0] === bodies[1] ? 'YES' : 'NO'}</div>
            `;
        });
    });
}

function testPost() {
    const results = document.getElementById('basic-results');
    const testData = JSON.stringify({test: 'data'});
    
    Promise.all([
        fetch('/api/test', {method: 'POST', body: testData}),
        fetch('http://localhost:8080/api/test', {method: 'POST', body: testData})
    ]).then(responses => {
        results.innerHTML += '<div>POST test completed - check console for details</div>';
    });
}

function performanceTest() {
    const results = document.getElementById('performance-results');
    results.innerHTML = 'Running performance test...';
    
    const requests = 100;
    const startTime = performance.now();
    
    // Test your webserv
    Promise.all(Array(requests).fill().map(() => fetch('/'))).then(() => {
        const webservTime = performance.now() - startTime;
        
        // Test nginx 1.1
        const nginxStart = performance.now();
        Promise.all(Array(requests).fill().map(() => 
            fetch('http://localhost:8080/')
        )).then(() => {
            const nginxTime = performance.now() - nginxStart;
            
            results.innerHTML = `
                <div>WebServ: ${webservTime.toFixed(2)}ms for ${requests} requests</div>
                <div>nginx 1.1: ${nginxTime.toFixed(2)}ms for ${requests} requests</div>
                <div>Ratio: ${(webservTime / nginxTime).toFixed(2)}x</div>
            `;
        });
    });
}

function testStaticFiles() {
    const files = ['/css/style.css', '/js/app.js', '/images/logo.png'];
    const results = document.getElementById('static-results');
    
    files.forEach(file => {
        Promise.all([
            fetch(file),
            fetch(`http://localhost:8080${file}`)
        ]).then(responses => {
            results.innerHTML += `<div>${file}: ${responses[0].status} vs ${responses[1].status}</div>`;
        });
    });
}
