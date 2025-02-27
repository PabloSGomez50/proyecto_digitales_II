import { Outlet } from 'react-router'
import './AuthLayout.css'


const AuthLayout = () => {

  return (
    <div className='auth-container'>
        <Outlet />
    </div>
  )
}

export default AuthLayout;
