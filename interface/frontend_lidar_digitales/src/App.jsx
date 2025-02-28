import { useEffect, useState } from 'react'
import { Route, Routes } from 'react-router';
import './App.css'

import Header from './components/Header/Header';
import Sidebar from './components/Sidebar/Sidebar';
import Dashboard from './components/Dashboard/Dashboard';

import { FaPlus, FaGear, FaDatabase } from "react-icons/fa6";
import { MdDashboard } from "react-icons/md";
import { GiDuel } from "react-icons/gi";
import Devices from './components/Devices/Devices';

const sidebarOptions = [
  {name: "Dashboard", icon: MdDashboard}, 
  // {name: "Devices", icon: FaDatabase},
  // {name: "Games", icon: GiDuel},
  // {name: "Events", icon: GiDuel},
];

function App() {
  const [loading, setLoading] = useState(true);
  const [optionSelected, setOptionSelected] = useState("dashboard");

  useEffect(() => {
    setTimeout(() => {
      setLoading(false);
    }, 500);
  }, []);

  return (
    <div className='main-container'>
      <Sidebar
        sidebarOptions={sidebarOptions}
        optionSelected={optionSelected}
        setOptionSelected={setOptionSelected}
        />
      {loading ?
        <div className='loading'>
          <p>Loading...</p>
          <div className="loader"></div>
        </div>
        :   
        <div>
          <Header />
          <section className='main-content'>
            <Routes>
              <Route index element={
                <Dashboard />
              } />
              <Route
                path="dashboard"
                element={<Dashboard />}
              />
              <Route path="devices" element={<Devices />}
              />
              <Route path="device/:id" element={<h1>Device</h1>}
              />
              {/*
              <Route path='games'>
                <Route index element={
                  <ContentForm title='games' Card={GameCard}>
                    
                  </ContentForm>
                }/>
              </Route> 
              */}

            </Routes>
          </section>
        </div>
      }
    </div>
  )
}

export default App;
