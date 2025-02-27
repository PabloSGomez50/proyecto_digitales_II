import { useEffect, useState } from 'react';
import { useUser } from '../../hooks/UserContext'
import { Outlet, Navigate  } from 'react-router'


const AuthReq = () => {

  const { user, retrieveUser } = useUser();
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    setLoading(!retrieveUser());
  }, []);

  return (
    <>
    {loading ?
      <p>
        Loading...
      </p>
    :
      <>
      {user?.email ?
        <Outlet />
        :
        <Navigate to="login" />
      }
      </>
    }
    
    </>
  )
}

export default AuthReq;
