const BASE_URL = '/session/auth.go'

function displayResult (elementId, data, isSuccess) {
  const element = document.getElementById(elementId)
  element.style.display = 'block'
  element.className = `result ${isSuccess ? 'success' : 'error'}`

  if (typeof data === 'object') {
    element.innerHTML = `<pre>${JSON.stringify(data, null, 2)}</pre>`
  } else {
    element.innerHTML = data
  }
}

function displayErrorLogin (message) {
  const status = document.getElementById('login-error')
  status.classList.remove('hidden')
  status.innerText = `${message}`
}

function setAuthUI (isAuthed) {
  var login = document.getElementById('login-section')
  var session = document.getElementById('session-section')
  var status = document.getElementById('auth-status')
  if (status) status.innerHTML = isAuthed ? 'Authenticated' : 'Checking...'
  if (login) login.classList.toggle('hidden', isAuthed)
  if (session) session.classList.toggle('hidden', !isAuthed)
}

async function login () {
  const usernameEl = document.getElementById('username')
  const passwordEl = document.getElementById('password')
  const username = usernameEl.value
  const password = passwordEl.value

  if (!username || !password) {
    displayResult('loginResult', 'Please fill all fields', false)
    return
  }

  // Loading state
  var btn = document.getElementById('login-button')
  var prevBtnHTML = null
  var prevBtnDisabled = false

  if (btn) {
    prevBtnHTML = btn.innerHTML
    prevBtnDisabled = btn.disabled
    btn.disabled = true
    btn.innerHTML = 'Signing in…'
    btn.classList.add('opacity-70', 'cursor-not-allowed')
  }

  if (usernameEl) usernameEl.disabled = true
  if (passwordEl) passwordEl.disabled = true
  var loginSection = document.getElementById('login-section')
  if (loginSection) loginSection.setAttribute('aria-busy', 'true')

  try {
    const response = await fetch(`${BASE_URL}?login`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ username, password })
    })

    const data = await response.json()
    console.log(data)
    if (data && data.ok) {
      usernameEl.value = ''
      passwordEl.value = ''
      document.getElementById('login-error').classList.add('hidden')
      setAuthUI(true)
      getUserInfo()
    } else {
      setAuthUI(false)
      displayErrorLogin(data.error || 'Internal server error')
    }
  } catch (error) {
    displayErrorLogin(error.message)
  } finally {
    // Loading state OFF
    if (btn) {
      btn.disabled = prevBtnDisabled
      btn.innerHTML = prevBtnHTML || 'Sign in'
      btn.classList.remove('opacity-70', 'cursor-not-allowed')
    }
    if (usernameEl) usernameEl.disabled = false
    if (passwordEl) passwordEl.disabled = false
    if (loginSection) loginSection.removeAttribute('aria-busy')
  }
}

function renderSessionView (data) {
  var userBox = document.getElementById('user-info-container')
  var detailsBox = document.getElementById('session-details-container')
  if (userBox) {
    userBox.innerHTML =
      '<div class="user-info">' +
      (data.username || '') +
      '</div>'
	}
  if (detailsBox && data.session_info) {
    var s = data.session_info
    detailsBox.innerHTML =
      '<div class="space-y-1">' +
      '<div><strong>Session ID:</strong> ' +
      (s.session_id || '') +
      '</div>' +
      '<div><strong>IP:</strong> ' +
      (s.ip_address || '') +
      '</div>' +
      '<div><strong>Créée le:</strong> ' +
      (s.created_at || '') +
      '</div>' +
      '<div><strong>Dernier accès:</strong> ' +
      (s.last_access || '') +
      '</div>' +
      '<div><strong>Durée session:</strong> ' +
      (s.session_duration || '') +
      '</div>' +
      '<div><strong>Sessions actives:</strong> ' +
      (s.total_sessions || '') +
      '</div>' +
      '</div>'
  }
}

function fill (id, value) {
  var el = document.getElementById(id);
  if (el) el.textContent = value || '—';
}

function clearSessionView () {
  fill('ui-username', '—');
  fill('ui-status', '—');
  fill('ui-session-id', '—');
  fill('ui-ip', '—');
  fill('ui-created', '—');
  fill('ui-last-access', '—');
  fill('ui-duration', '—');
  fill('ui-total', '—');
}

async function getUserInfo () {
	try {
	  const response = await fetch(`${BASE_URL}?user`, {
		method: 'GET',
		credentials: 'include'
	  })
  
	  const data = await response.json()
  
	  if (data && data.ok) {
		setAuthUI(true)
		fill('ui-username', data.username)
		fill('ui-status', 'Connected')
		var s = data.session_info || {}
		fill('ui-session-id', s.session_id)
		fill('ui-ip', s.ip_address)
		fill('ui-created', s.created_at)
		fill('ui-last-access', s.last_access)
		fill('ui-duration', s.session_duration)
		fill('ui-total', s.total_sessions != null ? String(s.total_sessions) : '')
		var err = document.getElementById('login-error')
		if (err) err.classList.add('hidden')
	  } else {
		clearSessionView()
		setAuthUI(false)
	  }
	} catch (error) {
	  clearSessionView()
	  setAuthUI(false)
	}
  }

  async function logout () {
	try {
	  const response = await fetch(`${BASE_URL}?logout`, {
		method: 'POST',
		credentials: 'include'
	  })
  
	  const data = await response.json()
  
	  if (data && data.ok) {
		clearSessionView()
		setAuthUI(false)
	  } else {
		// no-op
	  }
	} catch (error) {
	  console.error(error)
	}
  }

document.getElementById('password').addEventListener('keypress', function (e) {
  if (e.key === 'Enter') {
    login()
  }
})

window.onload = function () {
  getUserInfo()
}

