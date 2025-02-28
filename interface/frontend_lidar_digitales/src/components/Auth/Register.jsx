import './Register.css'
import { useState, useEffect } from 'react';
import { useUser } from '../../hooks/UserContext';
import { Link } from 'react-router';

import WilliamsLogo from '../../assets/imgs/Logo_Williams_F1.png';

const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;


const Register = () => {

  const { register } = useUser();
  const [email, setEmail] = useState('');
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [rpassword, setRPassword] = useState('');
  const [valid, setValid] = useState(false);
  const [error, setError] = useState('');

  const handleRegister = () => {
    register({
      email,
      username,
      password, 
      confirm_password: rpassword
    });
  }

  const checkValid = () => {
    const minChars = 4
    if (!emailRegex.test(email)){
      setValid(false);
      setError("El email ingresado no es valido");
      return;
    }

    if (username.length <= minChars) {
      setValid(false);
      setError(`El usuario debe tener al menos ${minChars} caracteres`);
      return;
    }
    
    if (password.length <= minChars) {
      setValid(false);
      setError(`La contraseña debe tener al menos ${minChars} caracteres`);
      return;
    }
    if (password !== rpassword) {
      setValid(false);
      setError("Las contraseñas deben coincidir");
      return;
    }
    
    setValid(true);
    setError("");
  }

  useEffect(() => {
    if (password.length > 0 & username.length > 0 & email.length > 0 & rpassword.length > 0) {
      checkValid();
    } else {
      setValid(false);
      setError("");
    }
  }, [email, username, password, rpassword])

  return (
    <div className='auth-form'>
      <div className='img-container'>
        <img src={WilliamsLogo}/>
      </div>
      <h2>MP competition</h2>
      <div className='inputs-container'>
        <input
          type="email"
          value={email}
          onChange={(e) => setEmail(e.target.value)}
          placeholder="Email"
        />
        <input
          type="text"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
          placeholder="Username"
        />
        <input
          type="password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          placeholder="Password"
        />
        <input
          type="password"
          value={rpassword}
          onChange={(e) => setRPassword(e.target.value)}
          placeholder="Confirm password"
        />
        {error &&
          <p className='error'>
            {error}
          </p>
        }
      </div>
      <button onClick={handleRegister} disabled={!valid}>Register</button>
      <Link to="/login">Login?</Link>
    </div>
  )
}

export default Register;
