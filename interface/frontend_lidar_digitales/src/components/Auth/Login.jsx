
import './Login.css'
import React, { useEffect, useState } from 'react';
import { useUser } from '../../hooks/UserContext';
import { Link } from 'react-router';

import WilliamsLogo from '../../assets/imgs/Logo_Williams_F1.png';

const Login = () => {
  const { login } = useUser();
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [valid, setValid] = useState(false);
  const [error, setError] = useState('');

  const handleLogin = (e) => {
    e.preventDefault();
    const logfunction = async () => {
      try {
        await login({ email_or_username: email, password });
      } catch (err) {
        console.log("error obj:", err);
        setError(err.response?.data?.detail || "Se produjo un error en el servidor")
      }
    }
    logfunction();
  };

  useEffect(() => {
    if (error.length > 0)
      setError("");
    setValid(email.length > 4 & password.length > 6);
  }, [email, password]);

  return (
    <div className='auth-form'>
      <div className='img-container'>
        <img src={WilliamsLogo}/>
      </div>
      <h2>MP competition</h2>
      <form className='inputs-container' onSubmit={handleLogin}>
        <input
          type="text"
          value={email}
          onChange={(e) => setEmail(e.target.value)}
          placeholder="Email or username"
        />
        <input
          type="password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          placeholder="Password"
        />


        <button onClick={handleLogin} disabled={!valid}>Login</button>
        {error.length > 0 &&
          <div className="error-container">
            <span>{error}</span>
          </div>
        }
      </form>
      <Link to="/register">Register?</Link>
    </div>
  );
};

export default Login;
